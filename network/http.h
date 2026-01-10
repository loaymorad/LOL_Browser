#pragma once
#include "url.h"
#include "socket.h"
#include "connection_pool.h"
#include <string>
#include <unordered_map>

enum class NetworkError {
    NONE,
    DNS,
    TCP,
    TLS,
    TIMEOUT,
    INVALID_RESPONSE
};

struct HttpResponse {
    int status  = 0; // 0 means "no HTTP response"
    std::unordered_map<std::string, std::string> headers;
    std::string body;
    NetworkError error = NetworkError::NONE;
};


class HTTPClient {
public:
    HTTPClient(ConnectionPool& pool);
    HttpResponse get(const URL& url, const std::string& ip);

private:
    ConnectionPool& pool;
};