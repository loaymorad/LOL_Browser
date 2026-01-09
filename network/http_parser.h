#pragma once
#include "http.h"
#include <string>

class HttpParser {
    public:
        static HttpResponse parse(const std::string& data);
};