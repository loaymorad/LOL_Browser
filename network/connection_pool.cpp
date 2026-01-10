#include "connection_pool.h"
#include <iostream>

using namespace std;

unique_ptr<Socket> ConnectionPool::get_connection(const string& host, int port) {
    string key = get_key(host, port);
    if (pool.find(key) != pool.end()) {
        auto socket = move(pool[key]);
        pool.erase(key);
        if (socket->is_connected()) {
            return socket;
        }
    }
    return nullptr;
}

void ConnectionPool::return_connection(const string& host, int port, unique_ptr<Socket> socket) {
    if (socket && socket->is_connected()) {
        string key = get_key(host, port);
        pool[key] = move(socket);
    }
}

string ConnectionPool::get_key(const string& host, int port) {
    return host + ":" + to_string(port);
}
