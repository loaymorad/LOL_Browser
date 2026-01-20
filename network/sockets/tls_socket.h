#pragma once
#include "socket.h"
#include <string>
#include <openssl/ssl.h>

class TLSSocket : public Socket {
public:
    TLSSocket();
    ~TLSSocket() override;

    bool connect(const std::string& ip, int port, const std::string& hostname = "") override;
    void send(const std::string& data) override;
    int read(char* buffer, size_t size) override;
    void close_socket() override;
    bool is_connected() const override;

private:
    int sockfd;

    // for https
    SSL_CTX* ctx;
    SSL* ssl;
};
