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

    // Load system default CA store
    if (!SSL_CTX_set_default_verify_paths(ctx)) {
        ERR_print_errors_fp(stderr);
    }

    // Enable certificate verification
    SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, nullptr);
}

TLSSocket::~TLSSocket() {
   close_socket();
   if(ctx) SSL_CTX_free(ctx);
}

bool TLSSocket::connect(const string& ip, int port) {
    return connect(ip, port, "");
}

bool TLSSocket::connect(const string& ip, int port, const string& hostname) {
    if (sockfd != -1) close_socket();

    sockfd = create_tcp_socket(ip, port);
    if (sockfd == -1) return false;

    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, sockfd);
    
    if (!hostname.empty()) {
        SSL_set_tlsext_host_name(ssl, hostname.c_str());
        // Enable hostname verification
        SSL_set1_host(ssl, hostname.c_str());
    }

    int result = SSL_connect(ssl);
    if(result != 1){
        ERR_print_errors_fp(stderr);
        return false;
    }

    return true;
}

void TLSSocket::send(const string& data) {
    if (!ssl) return;
    SSL_write(ssl, data.c_str(), data.size());
}

int TLSSocket::read(char* buffer, size_t size) {
    if (!ssl) return -1;
    return SSL_read(ssl, buffer, size);
}

void TLSSocket::close_socket() {
    if(ssl) {
        SSL_shutdown(ssl);
        SSL_free(ssl);
        ssl = nullptr;
    }
    if(sockfd != -1) {
        close(sockfd);
        sockfd = -1;
    }
}

bool TLSSocket::is_connected() const {
    return ssl != nullptr;
}
