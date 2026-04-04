#pragma once

#include <string>
#include <cstdint>

///
class IClient
{
public:
    ///
    virtual ~IClient() = default;

    ///
    virtual void connectServer(const std::string &host, uint16_t port) = 0;
    ///
    virtual void disconnect() = 0;

    ///
    virtual void sendText(const std::string &data) = 0;
    ///
    virtual std::string receive() = 0;
};

///
class TcpClient : public IClient
{
public:
    /// 
    TcpClient() = default;
    /// 
    ~TcpClient() override;

    /// 
    void connectServer(const std::string &host, uint16_t port) override;
    /// 
    void disconnect() override;

    /// 
    void sendText(const std::string &data) override;
    /// 
    std::string receive() override;

private:
    int sock_fd = -1; ///<
};