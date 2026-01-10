#pragma once
#include "socket.h"
#include <string>

class TCPSocket : public Socket {
public:
    TCPSocket();
    ~TCPSocket() override;

    bool connect(const std::string& ip, int port) override;
    void send(const std::string& data) override;
    int read(char* buffer, size_t size) override;
    void close_socket() override;
    bool is_connected() const override;

private:
    int sockfd;
};