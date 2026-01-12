#pragma once
#include "url.h"
#include "../sockets/socket.h"
#include "../sockets/connection_pool.h"
#include <string>
#include <unordered_map>
#include <memory>

// Forward declaration
class HTTPCache;

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
    ~HTTPClient();
    HttpResponse get(const URL& url, const std::string& ip);

private:
    ConnectionPool& pool;
    std::unique_ptr<HTTPCache> cache_;
    void send_request(Socket* socket, const URL& url);
    HttpResponse read_response_head(Socket* socket);
    std::string read_body(Socket* socket, const std::unordered_map<std::string, std::string>& headers, const std::string& initial_body_part);
};