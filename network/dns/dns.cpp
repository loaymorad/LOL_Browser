#include "dns.h"
#include <netdb.h>
#include <arpa/inet.h>
#include <iostream>

using namespace std;

string DNSResolver::resolve(const string& hostname) {
    if (hostname.empty()) {
        return "";
    }

    addrinfo hints{}, *res;
    hints.ai_family = AF_INET;

    if (getaddrinfo(hostname.c_str(), nullptr, &hints, &res) != 0) {
        cerr << "DNS Resolution failed for: " << hostname << endl;
        return "";
    }

    char ip[INET_ADDRSTRLEN];
    sockaddr_in* addr = (sockaddr_in*)res->ai_addr;
    inet_ntop(AF_INET, &addr->sin_addr, ip, sizeof(ip));

    freeaddrinfo(res);
    return ip;
}