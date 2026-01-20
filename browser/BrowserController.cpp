#include "BrowserController.h"
#include "../network/http/url.h"
#include "../network/dns/dns.h"
#include "../network/http/http.h"
#include "../render/text_renderer.h"
#include "error_page.h"

using namespace std;

constexpr int MAX_REDIRECTS = 5;

BrowserController::BrowserController() : http(pool) {}

void BrowserController::navigate(string& raw_url) {
    TextRenderer renderer;
    string result = fetch(raw_url);
    renderer.render(result);
}

string BrowserController::fetch(string& raw_url) {
    string current_url = raw_url;
    
    for(int i = 0; i < MAX_REDIRECTS; i++) {
        URL url(current_url);
        DNSResolver dns;
        
        string ip = dns.resolve(url.host());
        if (ip.empty()) {
            return ErrorPage::dns_error(url.host());
        }

        HttpResponse response = http.get(url, ip);

        if(response.error == NetworkError::TLS){
            return ErrorPage::tls_error();
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
            return ErrorPage::http_error(response.status);
        }

        // Success
        string output = "";
        for(auto const& [key, val] : response.headers) {
            output += key + ": " + val + "\n";
        }
        output += "\n" + response.body;
        return output;
    }

    return "Too many redirects";
}
