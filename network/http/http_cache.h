#pragma once
#include "http.h"
#include <string>
#include <unordered_map>
#include <chrono>

struct CachedResponse {
    HttpResponse response;
    std::chrono::time_point<std::chrono::steady_clock> cached_at;
    int max_age; // in seconds
};

class HTTPCache {
public:
    HTTPCache(int default_ttl_seconds = 60, size_t max_entries = 100);
    
    // Returns cached response or nullptr if not found/expired
    HttpResponse* get(const std::string& url);
    
    // Stores a response with caching headers
    void put(const std::string& url, const HttpResponse& response);
    
    // Clears all cached entries
    void clear();

private:
    int default_ttl_seconds_;
    size_t max_entries_;
    std::unordered_map<std::string, CachedResponse> cache_;
    
    bool is_expired(const CachedResponse& entry) const;
    bool is_cacheable(const HttpResponse& response) const;
    int parse_max_age(const HttpResponse& response) const;
    void evict_if_needed();
};
