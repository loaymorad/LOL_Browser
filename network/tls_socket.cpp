#include "tls_socket.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <openssl/err.h>

using namespace std;

TLSSocket::TLSSocket() : sockfd(-1), ctx(nullptr), ssl(nullptr) {
    SSL_library_init();
    SSL_load_error_strings();
    OpenSSL_add_all_algorithms();

    ctx = SSL_CTX_new(TLS_client_method());
}

TLSSocket::~TLSSocket() {
   if(ssl) SSL_free(ssl);
   if(ctx) SSL_CTX_free(ctx);
   if(sockfd != -1) close(sockfd);
}

bool TLSSocket::connect(const string& ip, int port, const string& hostname) {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) return false;

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    
    if (inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) <= 0) return false;
    
    if (::connect(sockfd, (sockaddr*)&addr, sizeof(addr)) != 0) return false;

    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, sockfd);
    SSL_set_tlsext_host_name(ssl, hostname.c_str());

    int result = SSL_connect(ssl);
    if(result != 1){
        ERR_print_errors_fp(stderr);
        return false;
    }

    return true;
}

void TLSSocket::send(const string& data) {
    if (ssl) {
        SSL_write(ssl, data.c_str(), data.size());
    }
}

string TLSSocket::receive() {
    string full_response;
    if (ssl) {
        char buffer[4096];
        while (true) {
            int bytes = SSL_read(ssl, buffer, sizeof(buffer) - 1);
            if (bytes > 0) {
                buffer[bytes] = '\0';
                full_response.append(buffer, bytes);
            } else {
                break;
            }
        }
    }
    return full_response;
}
