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

# TODOs


# --> Done
## 1. Proper HTTP Response Parsing [DONE]

## 2. Handle Redirects (3xx) [DONE]

## 3. Error Pages [DONE]
Instead of crashing:
  DNS error page
  TLS error page
  HTTP error page
# <-- Done

# --> Done
## 1. [TODO] Read Loop (Critical Bug) [DONE]
You only call recv() / SSL_read() once.
Real servers:
Send data in chunks
Large HTML = many reads

Real servers:
  Stream data
  Split responses across many TCP packets
  Use chunked transfer encoding
  Close connection only after full response

Read until the server closes the connection OR no more data is available
```
Connection: close
```

## 2. [TODO] HTTP Keep-Alive [DONE]
Reuse TCP/TLS connections:
Connection: keep-alive

With Keep-Alive
```
Connection: keep-alive
```
# <-- Done

# --> Done
## 3. Certificate Validation [DONE]
You need:
  Load CA store
  Verify certificate chain
  Match hostname

test:
Command: `./lol_browser https://expired.badssl.com`
Result: Connection failed with an OpenSSL error: certificate verify failed.
# <-- Done


# --> start
## 1. [TODO] Caching
DNS cache
HTTP cache
# <-- end

# --> start
refector and understand all code pieces
# <-- end

# --> start
## 1. [TODO] Simple UI
# <-- end

# --> start (after launching the browser)
## Respect Headers
Respect Cache-Control
CSP
cookies
Content-Type


## Render HTML
Recommended C++ Engine: LiteHTML
  A pure C++ HTML/CSS rendering engine with zero dependencies.
  
  Cons: No built-in JavaScript. You would need to pair it with a small JS engine like QuickJS or Duktape.

# <-- end
