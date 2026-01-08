#include "http.h"
#include "tcp_socket.h"
#include "tls_socket.h"

using namespace std;

HttpResponse HTTPClient::get(const URL& url, const string& ip) {
    HttpResponse http_response;
    
    if (ip.empty()) {
        http_response.status = 404;
        return http_response;
    }


    string response;


    if (url.scheme() == "https") {
        TLSSocket socket;
        if (!socket.connect(ip, url.port(), url.host())) {
            http_response.status = 500;
            return http_response;
        }

        socket.send("GET " + url.path() + " HTTP/1.1\r\nHost: " + url.host() + "\r\nUser-Agent: LOL_Browser/1.0\r\nConnection: close\r\n\r\n");
        response = socket.receive();
    }else {
        TCPSocket socket;
        if (!socket.connect(ip, url.port())) {
            http_response.status = 500;
            return http_response;
        }
        
        socket.send("GET " + url.path() + " HTTP/1.1\r\nHost: " + url.host() + "\r\nUser-Agent: LOL_Browser/1.0\r\nConnection: close\r\n\r\n");
        response = socket.receive();
    }

    
    
    
    http_response.status = 200;
    http_response.body = response;
    return http_response;
}
