#pragma once

#include "statistic_manager.h"

#include <vector>
#include <string>
#include <cstdint>
#include <thread>

/// Класс TCP-сервера
class TcpServer
{
public:
    /// Конструктор
    TcpServer(uint16_t port, const std::vector<std::string> &patterns);
    /// Деструктор
    ~TcpServer();

    /// Запустить сервер
    void run();

private:
    /// Обработать подключившегося клиента
    void handleClient(int clientFd);

    /// Запись статстики в FIFO для утилиты
    void threadFunc();

    int listen_sock_fd;                     ///< Файловый дескриптор для прослушивания
    std::vector<std::string> checkPatterns; ///< Паттерны, которые необходимо проверить
    StatisticManager statisticManager;      ///< Менеджер для работы со статистикой
    std::thread statsThread;                ///< Поток, в котором будет происходит запись в FIFO
};