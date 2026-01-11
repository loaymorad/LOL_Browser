#include "socket.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int Socket::create_tcp_socket(const std::string& ip, int port) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) return -1;

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) <= 0) {
        close(sockfd);
        return -1;
    }

    if (::connect(sockfd, (sockaddr*)&addr, sizeof(addr)) != 0) {
        close(sockfd);
        return -1;
    }

    return sockfd;
}
