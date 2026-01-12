#include "http_cache.h"
#include "http.h"
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;

HTTPCache::HTTPCache(int default_ttl_seconds, size_t max_entries)
    : default_ttl_seconds_(default_ttl_seconds), max_entries_(max_entries) {}

HttpResponse* HTTPCache::get(const string& url) {
    auto it = cache_.find(url);
    
    if (it == cache_.end()) {
        return nullptr; // Cache miss
    }
    
    if (is_expired(it->second)) {
        cache_.erase(it);
        return nullptr; // Expired entry
    }
    
    return &(it->second.response);
}

void HTTPCache::put(const string& url, const HttpResponse& response) {
    if (!is_cacheable(response)) {
        return; // Don't cache this response
    }
    
    evict_if_needed();
    
    CachedResponse entry;
    entry.response = response;
    entry.cached_at = chrono::steady_clock::now();
    entry.max_age = parse_max_age(response);
    
    cache_[url] = entry;
}

void HTTPCache::clear() {
    cache_.clear();
}

bool HTTPCache::is_expired(const CachedResponse& entry) const {
    auto now = chrono::steady_clock::now();
    auto elapsed = chrono::duration_cast<chrono::seconds>(now - entry.cached_at);
    return elapsed.count() >= entry.max_age;
}

bool HTTPCache::is_cacheable(const HttpResponse& response) const {
    // Only cache successful responses
    if (response.status != 200) {
        return false;
    }
    
    // Check for Cache-Control: no-store or no-cache
    auto cc_it = response.headers.find("Cache-Control");
    if (cc_it != response.headers.end()) {
        string cache_control = cc_it->second;
        // Convert to lowercase for case-insensitive comparison
        transform(cache_control.begin(), cache_control.end(), cache_control.begin(), ::tolower);
        
        if (cache_control.find("no-store") != string::npos || 
            cache_control.find("no-cache") != string::npos) {
            return false;
        }
    }
    
    return true;
}

int HTTPCache::parse_max_age(const HttpResponse& response) const {
    auto cc_it = response.headers.find("Cache-Control");
    if (cc_it != response.headers.end()) {
        string cache_control = cc_it->second;
        
        // Look for max-age directive
        size_t pos = cache_control.find("max-age=");
        if (pos != string::npos) {
            pos += 8; // length of "max-age="
            size_t end = cache_control.find_first_of(", ", pos);
            string max_age_str = cache_control.substr(pos, end - pos);
            
            try {
                return stoi(max_age_str);
            } catch (...) {
                // If parsing fails, use default
            }
        }
    }
    
    return default_ttl_seconds_;
}

void HTTPCache::evict_if_needed() {
    if (cache_.size() >= max_entries_) {
        // Simple eviction: remove the first entry
        if (!cache_.empty()) {
            cache_.erase(cache_.begin());
        }
    }
}
