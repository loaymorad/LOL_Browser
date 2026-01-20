QT += core gui widgets
TARGET = lol_browser_gui
TEMPLATE = app

# Define C++ standard
CONFIG += c++17

# Source files
SOURCES += main.cpp \
           ui/qt/mainwindow.cpp \
           browser/BrowserController.cpp \
           browser/error_page.cpp \
           network/dns/dns.cpp \
           network/dns/dns_cache.cpp \
           network/sockets/socket.cpp \
           network/http/http.cpp \
           network/http/http_cache.cpp \
           network/http/http_parser.cpp \
           network/sockets/tcp_socket.cpp \
           network/sockets/tls_socket.cpp \
           network/http/url.cpp \
           network/sockets/connection_pool.cpp \
           render/text_renderer.cpp

# Header files
HEADERS += ui/qt/mainwindow.h \
           browser/BrowserController.h \
           browser/error_page.h \
           network/dns/dns.h \
           network/dns/dns_cache.h \
           network/sockets/socket.h \
           network/http/http.h \
           network/http/http_cache.h \
           network/http/http_parser.h \
           network/sockets/tcp_socket.h \
           network/sockets/tls_socket.h \
           network/http/url.h \
           network/sockets/connection_pool.h \
           render/text_renderer.h

# Link against OpenSSL
LIBS += -lssl -lcrypto
