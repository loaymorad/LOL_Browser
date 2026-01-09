#include "http.h"
#include "http_parser.h"
#include "tcp_socket.h"
#include "tls_socket.h"

using namespace std;

HttpResponse HTTPClient::get(const URL& url, const string& ip) {
    string raw_res;

    if (url.scheme() == "https") {
        TLSSocket socket;
        if (!socket.connect(ip, url.port(), url.host()))
            return {0, {}, "", NetworkError::TLS};

        socket.send("GET " + url.path() + " HTTP/1.1\r\nHost: " + url.host() + "\r\nUser-Agent: LOL_Browser/1.0\r\nConnection: close\r\n\r\n");
        raw_res = socket.receive();
    
    }else {
        TCPSocket socket;
        if (!socket.connect(ip, url.port())) {
            return {0, {}, "", NetworkError::TCP};
        }
        
        socket.send("GET " + url.path() + " HTTP/1.1\r\nHost: " + url.host() + "\r\nUser-Agent: LOL_Browser/1.0\r\nConnection: close\r\n\r\n");
        raw_res = socket.receive();
    }

    return HttpParser::parse(raw_res);
}
