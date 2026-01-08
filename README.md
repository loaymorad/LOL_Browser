current version
```vbnet
main
 ↓
CLI → get_url
 ↓
BrowserController::navigate
 ↓
URL parse
 ↓
DNS resolve
 ↓
TCP connect
 ↓
HTTP GET
 ↓
Response
 ↓
Text render
```


```
g++ -std=c++17 main.cpp ui/cli.cpp browser/controller.cpp network/url.cpp network/dns.cpp network/tcp_socket.cpp network/http.cpp render/text_renderer.cpp -o browser
```

# Add HTTPS with OpenSSL
HTTPS is NOT a new protocol.
```
HTTP
↓
TLS (encryption)
↓
TCP
↓
IP
```
What’s missing for HTTPS is ONLY ONE THING: A TLS layer that sits on top of your existing TCP socket
So your stack becomes:
```
HTTPClient
  ↓
TLSSocket (NEW)
  ↓
TCPSocket
  ↓
TCP
```

HTTP will say:
    If scheme == https → use TLSSocket
    Else → use TCPSocket


```
sudo apt install libssl-dev
```

compile
```
g++ *.cpp -lssl -lcrypto
```


# [TODO]