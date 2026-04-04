#include "server.h"
#include "pattern_matcher.h"

#include <atomic>
#include <csignal>
#include <sys/socket.h>
#include <stdexcept>
#include <netinet/in.h>
#include <iostream>
#include <sys/wait.h>

namespace
{
    std::atomic<bool> g_running{true};

    void signalHandler(int sig)
    {
        g_running.store(false);
    }
}

TcpServer::TcpServer(uint16_t port, const std::vector<std::string> &patterns) : listen_sock_fd(-1), checkPatterns(patterns), statisticManager(patterns)
{
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);
    std::signal(SIGCHLD, [](int sig)
                { while (waitpid(-1, nullptr, WNOHANG) > 0); });

    listen_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock_fd < 0)
    {
        throw std::runtime_error("Не удалось создать сокет для прослушивания");
    }

    int opt = 1;
    setsockopt(listen_sock_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(listen_sock_fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) < 0)
    {
        throw std::runtime_error("Не удалось привязать порт к сокету: " + std::to_string(port));
    }

    if (listen(listen_sock_fd, SOMAXCONN) < 0)
    {
        throw std::runtime_error("Не удалось установить прослушивание для сокета");
    }
}

TcpServer::~TcpServer()
{
    if (listen_sock_fd != -1)
    {
        close(listen_sock_fd);
        listen_sock_fd = -1;
    }
}

void TcpServer::run()
{
    while (g_running.load())
    {
        int clientFd = accept(listen_sock_fd, nullptr, nullptr);
        if (clientFd < 0)
        {
            continue;
        }

        pid_t pid = fork();
        if (pid < 0)
        {
            std::cerr << "Не удалось создать дочерний процесс" << std::endl;
            close(clientFd);
            continue;
        }

        if (pid == 0)
        {
            close(listen_sock_fd);
            handleClient(clientFd);
            exit(0);
        }

        close(clientFd);
    }

    while (waitpid(-1, nullptr, 0) > 0)
        ;
}

void TcpServer::handleClient(int clientFd)
{
    uint32_t netSize = 0;
    ssize_t n = recv(clientFd, &netSize, sizeof(netSize), MSG_WAITALL);
    if (n != sizeof(netSize))
    {
        close(clientFd);
        return;
    }
    uint32_t fileSize = ntohl(netSize);

    std::string content(fileSize, '\0');
    ssize_t received = recv(clientFd, content.data(), fileSize, MSG_WAITALL);
    if (received != static_cast<ssize_t>(fileSize))
    {
        close(clientFd);
        return;
    }

    PatternMatcher matcher{checkPatterns};
    auto matches = matcher.match(content);

    std::string response;
    if (matches.empty())
    {
        response = "FILE IS CLEAN\n";
    }
    else
    {
        response = "INFECTED:";
        for (const auto &[pattern, count] : matches)
        {
            response += " " + pattern + "(" + std::to_string(count) + ")";
        }
        response += "\n";
    }

    send(clientFd, response.c_str(), response.size(), 0);
    statisticManager.update(matches);

    close(clientFd);
}