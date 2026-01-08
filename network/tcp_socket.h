#pragma once
#include <string>

class TCPSocket {
public:
    TCPSocket();
    ~TCPSocket();
    bool connect(const std::string& ip, int port);
    void send(const std::string& data);
    std::string receive();

private:
    int sockfd;
};