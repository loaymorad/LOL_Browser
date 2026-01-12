#pragma once
#include <string>
#include <unordered_map>
#include <chrono>

struct DNSCacheEntry {
    std::string ip;
    std::chrono::time_point<std::chrono::steady_clock> cached_at;
};

class DNSCache {
public:
    DNSCache(int ttl_seconds = 300, size_t max_entries = 1000);
    
    // Returns cached IP or empty string if not found/expired
    std::string get(const std::string& hostname);
    
    // Stores a DNS resolution with current timestamp
    void put(const std::string& hostname, const std::string& ip);
    
    // Clears all cached entries
    void clear();

private:
    int ttl_seconds_;
    size_t max_entries_;
    std::unordered_map<std::string, DNSCacheEntry> cache_;
    
    bool is_expired(const DNSCacheEntry& entry) const;
    void evict_if_needed();
};
