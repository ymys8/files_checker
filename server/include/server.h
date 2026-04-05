#pragma once

#include "statistic_manager.h"

#include <vector>
#include <string>
#include <cstdint>
#include <thread>

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

    ///
    void threadFunc();

    int listen_sock_fd;                     ///<
    std::vector<std::string> checkPatterns; ///<
    StatisticManager statisticManager;      ///<
    std::thread statsThread;    ///< 
};