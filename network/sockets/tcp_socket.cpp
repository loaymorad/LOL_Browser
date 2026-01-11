#include "tcp_socket.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

TCPSocket::TCPSocket() : sockfd(-1) {}

TCPSocket::~TCPSocket() {
    close_socket();
}

bool TCPSocket::connect(const string& ip, int port) {
    if (sockfd != -1) close_socket();

    sockfd = create_tcp_socket(ip, port);
    return sockfd != -1;
}

void TCPSocket::send(const string& data) {
    if (sockfd != -1) {
        ::send(sockfd, data.c_str(), data.size(), 0);
    }
}

int TCPSocket::read(char* buffer, size_t size) {
    if (sockfd == -1) return -1;
    return ::recv(sockfd, buffer, size, 0);
}

void TCPSocket::close_socket() {
    if (sockfd != -1) {
        close(sockfd);
        sockfd = -1;
    }
}

bool TCPSocket::is_connected() const {
    return sockfd != -1;
}
