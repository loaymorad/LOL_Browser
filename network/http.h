#pragma once
#include "url.h"
#include "tcp_socket.h"
#include <string>

struct HttpResponse {
    int status;
    std::string body;
};


class HTTPClient {
public:
    HttpResponse get(const URL& url, const std::string& ip);
};