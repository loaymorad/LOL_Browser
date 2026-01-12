#pragma once
#include <string>
#include "dns_cache.h"

class DNSResolver {
public:
    std::string resolve(const std::string& hostname);

private:
    DNSCache cache_;
};