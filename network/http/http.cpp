#include "http.h"
#include "http_cache.h"
#include "http_parser.h"
#include "../sockets/tcp_socket.h"
#include "../sockets/tls_socket.h"
#include <iostream>
#include <sstream>

using namespace std;

HTTPClient::HTTPClient(ConnectionPool& pool) : pool(pool), cache_(std::make_unique<HTTPCache>()) {}

HTTPClient::~HTTPClient() = default;

HttpResponse HTTPClient::get(const URL& url, const string& ip) {
    // Check cache first
    string cache_key = url.to_string();
    HttpResponse* cached = cache_->get(cache_key);
    if (cached != nullptr) {
        return *cached;
    }

    // Cache miss - proceed with normal request
    unique_ptr<Socket> socket = pool.get_connection(url.host(), url.port());
    bool reused = (socket != nullptr);

    auto connect_socket = [&]() -> unique_ptr<Socket> {
        if (url.scheme() == "https") {
            auto tls_socket = make_unique<TLSSocket>();
            if (tls_socket->connect(ip, url.port(), url.host())) return tls_socket;
        } else {
            auto tcp_socket = make_unique<TCPSocket>();
            if (tcp_socket->connect(ip, url.port())) return tcp_socket;
        }
        return nullptr;
    };

    if (!socket) {
        socket = connect_socket();
        if (!socket) return {0, {}, "", (url.scheme() == "https" ? NetworkError::TLS : NetworkError::TCP)};
    }

    // Send Request
    send_request(socket.get(), url);

    HttpResponse response = read_response_head(socket.get());
    if (response.error == NetworkError::NONE && response.status != 0) {
        response.body = read_body(socket.get(), response.headers, response.body);
    }

    // Retry logic for reused connections that might have been closed by server
    if (reused && (response.status == 0 || response.error != NetworkError::NONE)) {
        socket->close_socket();
        socket = connect_socket();
        if (!socket) return {0, {}, "", (url.scheme() == "https" ? NetworkError::TLS : NetworkError::TCP)};
        
        send_request(socket.get(), url);
        response = read_response_head(socket.get());
        if (response.error == NetworkError::NONE && response.status != 0) {
            response.body = read_body(socket.get(), response.headers, response.body);
        }
    }

    // Keep-Alive Logic
    bool keep_alive = true;
    if (response.headers.count("Connection") && response.headers["Connection"] == "close") {
        keep_alive = false;
    }

    if (keep_alive && response.status != 0) {
        pool.return_connection(url.host(), url.port(), move(socket));
    } else {
        socket->close_socket();
    }

    // Store in cache if successful
    if (response.error == NetworkError::NONE && response.status == 200) {
        cache_->put(cache_key, response);
    }

    return response;
}

void HTTPClient::send_request(Socket* socket, const URL& url) {
    socket->send("GET " + url.path() + " HTTP/1.1\r\nHost: " + url.host() + "\r\nUser-Agent: LOL_Browser/1.0\r\nConnection: keep-alive\r\n\r\n");
}

HttpResponse HTTPClient::read_response_head(Socket* socket) {
    string raw_data;
    char buffer[4096];
    bool headers_received = false;
    size_t header_end_pos = string::npos;

    // 1. Read until headers are complete
    while (!headers_received) {
        int bytes = socket->read(buffer, sizeof(buffer));
        if (bytes <= 0) break; // Connection closed or error
        raw_data.append(buffer, bytes);
        
        header_end_pos = raw_data.find("\r\n\r\n");
        if (header_end_pos != string::npos) {
            headers_received = true;
        }
    }

    if (!headers_received) return {0, {}, "", NetworkError::INVALID_RESPONSE};

    // 2. Parse Headers
    string header_part = raw_data.substr(0, header_end_pos + 4);
    HttpResponse res = HttpParser::parse(header_part);
    
    // Store the initial part of the body that was read with headers
    res.body = raw_data.substr(header_end_pos + 4);
    
    return res;
}

std::string HTTPClient::read_body(Socket* socket, const std::unordered_map<std::string, std::string>& headers, const std::string& initial_body_part) {
    string body_part = initial_body_part;
    char buffer[4096];

    if (headers.count("Content-Length")) {
        int content_length = stoi(headers.at("Content-Length"));
        int remaining = content_length - body_part.size();
        
        while (remaining > 0) {
            int to_read = min(remaining, (int)sizeof(buffer));
            int bytes = socket->read(buffer, to_read);
            if (bytes <= 0) break;
            body_part.append(buffer, bytes);
            remaining -= bytes;
        }
    } else if (headers.count("Transfer-Encoding") && headers.at("Transfer-Encoding") == "chunked") {
        string full_body;
        string current_buffer = body_part;
        
        while (true) {
            // Find chunk size line
            size_t crlf = current_buffer.find("\r\n");
            while (crlf == string::npos) {
                int bytes = socket->read(buffer, sizeof(buffer));
                if (bytes <= 0) break;
                current_buffer.append(buffer, bytes);
                crlf = current_buffer.find("\r\n");
            }
            if (crlf == string::npos) break; // Error or incomplete

            string size_str = current_buffer.substr(0, crlf);
            int chunk_size = 0;
            stringstream ss;
            ss << hex << size_str;
            ss >> chunk_size;
            
            // Remove size line + CRLF
            current_buffer.erase(0, crlf + 2);

            if (chunk_size == 0) {
                // Read trailing CRLF
                if (current_buffer.size() < 2) {
                     while (current_buffer.size() < 2) {
                        int bytes = socket->read(buffer, sizeof(buffer));
                         if (bytes <= 0) break;
                        current_buffer.append(buffer, bytes);
                     }
                }
                break; // Done
            }

            // Read chunk data
            while (current_buffer.size() < (size_t)chunk_size + 2) { // +2 for trailing CRLF
                int bytes = socket->read(buffer, sizeof(buffer));
                if (bytes <= 0) break;
                current_buffer.append(buffer, bytes);
            }
            
            full_body.append(current_buffer.substr(0, chunk_size));
            current_buffer.erase(0, chunk_size + 2); // Remove data + CRLF
        }
        body_part = full_body;
    }

    return body_part;
}

