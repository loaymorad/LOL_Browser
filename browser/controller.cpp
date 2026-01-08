#include "controller.h"
#include "../network/url.h"
#include "../network/dns.h"
#include "../network/http.h"
#include "../render/text_renderer.h"
#include <iostream>

using namespace std;
//  > Entry Point <

/*
input: http://www.vulnweb.com
> `URL url(raw_url);`
    url
        scheme: http
        host: www.vulnweb.com
        path: /
        port: 80

> `DNSResolver dns; dns.resolve("www.vulnweb.com");`

*/

void BrowserController::navigate(string& raw_url) {
    URL url(raw_url);
    DNSResolver dns;
    HTTPClient http;
    TextRenderer renderer;
    
    string ip = dns.resolve(url.host());
    HttpResponse response = http.get(url, ip);
    renderer.render(response.body);
}
