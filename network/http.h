#pragma once
#include "url.h"
#include "tcp_socket.h"
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
    HttpResponse get(const URL& url, const std::string& ip);
};