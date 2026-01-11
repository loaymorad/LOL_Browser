#include "controller.h"
#include "../network/http/url.h"
#include "../network/dns/dns.h"
#include "../network/http/http.h"
#include "../render/text_renderer.h"
#include "error_page.h"
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

constexpr int MAX_REDIRECTS = 5;

BrowserController::BrowserController() : http(pool) {}

void BrowserController::navigate(string& raw_url) {
    TextRenderer renderer;
    string current_url = raw_url;
    
    for(int i = 0; i < MAX_REDIRECTS; i++) {
        URL url(current_url);
        DNSResolver dns;
        
        string ip = dns.resolve(url.host());
        if (ip.empty()) {
            renderer.render(ErrorPage::dns_error(url.host()));
            return;
        }

        HttpResponse response = http.get(url, ip);

        if(response.error == NetworkError::TLS){
            renderer.render(ErrorPage::tls_error());
            return;
        }

        // Handle Redirects
        if (response.status >= 300 && response.status < 400) {
            auto location = response.headers.find("Location");
            if(location != response.headers.end()){
                current_url = location->second;
                continue;
            }
        }

        // Handle HTTP Errors
        if (response.error != NetworkError::NONE || response.status >= 400) {
            renderer.render(ErrorPage::http_error(response.status));
            return;
        }

        // Success
        string output = "";
        for(auto const& [key, val] : response.headers) {
            output += key + ": " + val + "\n";
        }
        output += "\n" + response.body;
        renderer.render(output);
        return;
    }

    renderer.render("Too many redirects");
}
