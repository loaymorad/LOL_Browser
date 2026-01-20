#include "dns_cache.h"

using namespace std;

DNSCache::DNSCache(int ttl_seconds, size_t max_entries) 
    : ttl_seconds_(ttl_seconds), max_entries_(max_entries) {}

string DNSCache::get(const string& hostname) {
    auto it = cache_.find(hostname);
    
    if (it == cache_.end()) {
        return ""; // Cache miss
    }
    
    if (is_expired(it->second)) {
        cache_.erase(it);
        return ""; // Expired entry
    }
    
    return it->second.ip;
}

void DNSCache::put(const string& hostname, const string& ip) {
    if (ip.empty()) {
        return; // Don't cache failed resolutions
    }
    
    evict_if_needed();
    
    DNSCacheEntry entry;
    entry.ip = ip;
    entry.cached_at = chrono::steady_clock::now();
    
    cache_[hostname] = entry;
}


bool DNSCache::is_expired(const DNSCacheEntry& entry) const {
    auto now = chrono::steady_clock::now();
    auto elapsed = chrono::duration_cast<chrono::seconds>(now - entry.cached_at);
    return elapsed.count() >= ttl_seconds_;
}

void DNSCache::evict_if_needed() {
    if (cache_.size() >= max_entries_) {
        // Simple eviction: remove the first entry (not true LRU, but good enough)
        if (!cache_.empty()) {
            cache_.erase(cache_.begin());
        }
    }
}
