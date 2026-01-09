CXX = g++
CXXFLAGS = -Wall -std=c++17
LDFLAGS = -lssl -lcrypto

SRCS = main.cpp \
       browser/controller.cpp \
       network/dns.cpp \
       network/http.cpp \
       network/http_parser.cpp \
       network/tcp_socket.cpp \
       network/tls_socket.cpp \
       network/url.cpp \
       network/error_page.cpp \
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
