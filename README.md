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

