#include "http.h"
#include "http_parser.h"
#include "tcp_socket.h"
#include "tls_socket.h"
#include <iostream>
#include <sstream>

using namespace std;

HTTPClient::HTTPClient(ConnectionPool& pool) : pool(pool) {}

HttpResponse HTTPClient::get(const URL& url, const string& ip) {
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
    socket->send("GET " + url.path() + " HTTP/1.1\r\nHost: " + url.host() + "\r\nUser-Agent: LOL_Browser/1.0\r\nConnection: keep-alive\r\n\r\n");

    auto read_response = [&](Socket* sock) -> HttpResponse {
        string raw_data;
        char buffer[4096];
        bool headers_received = false;
        size_t header_end_pos = string::npos;

        // 1. Read until headers are complete
        while (!headers_received) {
            int bytes = sock->read(buffer, sizeof(buffer));
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
        
        // 3. Read Body
        string body_part = raw_data.substr(header_end_pos + 4);
        
        if (res.headers.count("Content-Length")) {
            int content_length = stoi(res.headers["Content-Length"]);
            int remaining = content_length - body_part.size();
            
            while (remaining > 0) {
                int to_read = min(remaining, (int)sizeof(buffer));
                int bytes = sock->read(buffer, to_read);
                if (bytes <= 0) break;
                body_part.append(buffer, bytes);
                remaining -= bytes;
            }
        } else if (res.headers.count("Transfer-Encoding") && res.headers["Transfer-Encoding"] == "chunked") {
            // Process chunked encoding
            // This is a simplified chunked reader that assumes we might have partial chunks in body_part
            // For robustness, a proper state machine or a specialized stream reader is better, 
            // but we'll stick to a basic implementation that reads until 0-sized chunk.
            
            // We need to process 'body_part' and potentially read more from 'sock'
            // To keep it simple for this refactor without rewriting a full chunk parser:
            // We will read everything until the connection closes OR we find the end of chunks.
            // Note: This is tricky with Keep-Alive if we don't parse chunks strictly.
            // Let's implement a stricter loop that consumes body_part and reads more as needed.
            
            string full_body;
            string current_buffer = body_part;
            
            while (true) {
                // Find chunk size line
                size_t crlf = current_buffer.find("\r\n");
                while (crlf == string::npos) {
                    int bytes = sock->read(buffer, sizeof(buffer));
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
                            int bytes = sock->read(buffer, sizeof(buffer));
                             if (bytes <= 0) break;
                            current_buffer.append(buffer, bytes);
                         }
                    }
                    break; // Done
                }

                // Read chunk data
                while (current_buffer.size() < (size_t)chunk_size + 2) { // +2 for trailing CRLF
                    int bytes = sock->read(buffer, sizeof(buffer));
                    if (bytes <= 0) break;
                    current_buffer.append(buffer, bytes);
                }
                
                full_body.append(current_buffer.substr(0, chunk_size));
                current_buffer.erase(0, chunk_size + 2); // Remove data + CRLF
            }
            body_part = full_body;
        }

        res.body = body_part;
        return res;
    };

    HttpResponse response = read_response(socket.get());

    // Retry logic for reused connections that might have been closed by server
    if (reused && (response.status == 0 || response.error != NetworkError::NONE)) {
        socket->close_socket();
        socket = connect_socket();
        if (!socket) return {0, {}, "", (url.scheme() == "https" ? NetworkError::TLS : NetworkError::TCP)};
        
        socket->send("GET " + url.path() + " HTTP/1.1\r\nHost: " + url.host() + "\r\nUser-Agent: LOL_Browser/1.0\r\nConnection: keep-alive\r\n\r\n");
        response = read_response(socket.get());
    }

    // Keep-Alive Logic
    bool keep_alive = true;
    if (response.headers.count("Connection") && response.headers["Connection"] == "close") {
        keep_alive = false;
    }
    // HTTP/1.0 defaults to close unless keep-alive is specified, but we sent HTTP/1.1
    // HTTP/1.1 defaults to keep-alive unless close is specified.

    if (keep_alive && response.status != 0) {
        pool.return_connection(url.host(), url.port(), move(socket));
    } else {
        socket->close_socket();
    }

    return response;
}
