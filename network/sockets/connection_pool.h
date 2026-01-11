#pragma once
#include "socket.h"
#include <string>
#include <unordered_map>
#include <memory>

class ConnectionPool {
public:
    // Returns a connected socket if available, or nullptr.
    // The caller takes ownership of the socket temporarily.
    std::unique_ptr<Socket> get_connection(const std::string& host, int port);

    // Returns a socket to the pool for future reuse.
    void return_connection(const std::string& host, int port, std::unique_ptr<Socket> socket);

private:
    std::string get_key(const std::string& host, int port);
    std::unordered_map<std::string, std::unique_ptr<Socket>> pool;
};
