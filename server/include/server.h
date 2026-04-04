#pragma once

#include "statistic_manager.h"

#include <vector>
#include <string>
#include <cstdint>

///
class TcpServer
{
public:
    ///
    TcpServer(uint16_t port, const std::vector<std::string> &patterns);
    ///
    ~TcpServer();

    ///
    void run();

private:
    ///
    void handleClient(int clientFd);

    int listen_sock_fd;                     ///<
    std::vector<std::string> checkPatterns; ///<
    StatisticManager statisticManager; ///<
};