#pragma once
#include <string>

class DNSResolver {
public:
    std::string resolve(const std::string& hostname);
};