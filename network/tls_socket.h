#pragma once
#include <string>
#include <openssl/ssl.h>

class TLSSocket {
public:
    TLSSocket();
    ~TLSSocket();

    bool connect(const std::string& ip, int port, const std::string& hostname);
    void send(const std::string& data);
    std::string receive();

private:
    int sockfd;

    // for https
    SSL_CTX* ctx;
    SSL* ssl;
};
