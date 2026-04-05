#include "client.h"

#include <sys/socket.h>
#include <stdexcept>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

TcpClient::~TcpClient()
{
    disconnect();
}

void TcpClient::connectServer(const std::string &host, uint16_t port)
{
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0)
    {
        throw std::runtime_error("Не удалось создать сокет");
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    if (inet_pton(AF_INET, host.c_str(), &addr.sin_addr) <= 0)
    {
        throw std::runtime_error("Передали неправильный адрес хоста: " + host);
    }

    if (connect(sock_fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) < 0)
    {
        throw std::runtime_error("Не удалось подключиться к " + host + ":" + std::to_string(port));
    }
}

void TcpClient::disconnect()
{
    if (sock_fd >= 0)
    {
        close(sock_fd);
        sock_fd = -1;
    }
}

void TcpClient::sendText(const std::string &data)
{
    uint32_t size = htonl(static_cast<uint32_t>(data.size()));
    if (send(sock_fd, &size, sizeof(size), 0) < 0)
    {
        throw std::runtime_error("Не удалось отправить размер данных");
    }

    size_t totalSent = 0;
    while (totalSent < data.size())
    {
        ssize_t sent = send(sock_fd, data.c_str() + totalSent, data.size() - totalSent, 0);
        if (sent < 0)
        {
            throw std::runtime_error("Не удалось отправить часть данных");
        }
        totalSent += sent;
    }
}

std::string TcpClient::receive()
{
    std::string response;
    char buf[256];

    while (true)
    {
        ssize_t n = recv(sock_fd, buf, sizeof(buf) - 1, 0);
        if (n < 0)
        {
            throw std::runtime_error("Ошибка при получении ответа");
        }
        else if (n == 0)
        {
            break;
        }

        buf[n] = '\0';
        response.append(buf);

        // Решил, что ответ сервера должен заканчиваться переносом строки
        if (response.back() == '\n')
        {
            break;
        }
    }

    return response;
}