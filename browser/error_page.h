#pragma once
#include <string>

class ErrorPage {
    public:
        static std::string dns_error(const std::string& host);
        static std::string tls_error();
        static std::string http_error(int status);
};