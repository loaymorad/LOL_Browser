#include "tcp_socket.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

TCPSocket::TCPSocket() : sockfd(-1) {}

TCPSocket::~TCPSocket() {
    if (sockfd != -1) {
        close(sockfd);
    }
}

bool TCPSocket::connect(const string& ip, int port) {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) return false;

    sockaddr_in addr;
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    
    if (inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) <= 0) return false;
    
    return ::connect(sockfd, (sockaddr*)&addr, sizeof(addr)) == 0;
}

void TCPSocket::send(const string& data) {
    if (sockfd != -1) {
        ::send(sockfd, data.c_str(), data.size(), 0);
    }
}

string TCPSocket::receive() {
    if (sockfd == -1) return "";
    
    char buffer[4096];
    int bytes = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
    if (bytes > 0) {
        buffer[bytes] = '\0';
        return string(buffer);
    }
    return "";
}
