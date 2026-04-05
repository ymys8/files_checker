#pragma once

#include <string>
#include <cstdint>

/// Абстрактный класс с интерфейсом клиента
class IClient
{
public:
    /// Виртуальный деструктор
    virtual ~IClient() = default;

    /// Подключиться к серверу
    virtual void connectServer(const std::string &host, uint16_t port) = 0;
    /// Отключиться от сервера
    virtual void disconnect() = 0;

    /// Отправить текст файла на сервер
    virtual void sendText(const std::string &data) = 0;
    /// Получить ответ от сервера
    virtual std::string receive() = 0;
};

/// Класс с реализацией TCP-клиента
class TcpClient : public IClient
{
public:
    /// Конструктор
    TcpClient() = default;
    /// Деструктор
    ~TcpClient() override;

    /// Подключиться к серверу
    void connectServer(const std::string &host, uint16_t port) override;
    /// Отключиться от сервера
    void disconnect() override;

    /// Отправить текст файла на сервер
    void sendText(const std::string &data) override;
    /// Получить ответ от сервера
    std::string receive() override;

private:
    int sock_fd = -1; ///< Файловый дескриптор для серверного сокета
};