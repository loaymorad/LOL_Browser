CXX = g++
CXXFLAGS = -Wall -std=c++17
LDFLAGS = -lssl -lcrypto

SRCS = main.cpp \
       browser/controller.cpp \
       browser/error_page.cpp \
       network/dns/dns.cpp \
       network/sockets/socket.cpp \
       network/http/http.cpp \
       network/http/http_parser.cpp \
       network/sockets/tcp_socket.cpp \
       network/sockets/tls_socket.cpp \
       network/http/url.cpp \
       network/sockets/connection_pool.cpp \
       render/text_renderer.cpp \
       ui/cli.cpp

OBJS = $(SRCS:.cpp=.o)
TARGET = lol_browser

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
