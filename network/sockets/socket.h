#pragma once
#include <string>

class Socket {
public:
    virtual ~Socket() = default;
    virtual bool connect(const std::string& ip, int port) = 0;
    virtual void send(const std::string& data) = 0;
    // Reads up to size bytes into buffer. Returns number of bytes read.
    virtual int read(char* buffer, size_t size) = 0;
    virtual void close_socket() = 0;
    virtual bool is_connected() const = 0;

protected:
    int create_tcp_socket(const std::string& ip, int port);
};
