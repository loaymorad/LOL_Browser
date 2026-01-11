#include "error_page.h"

using namespace std;

string ErrorPage::dns_error(const string& host) {
    return "<html><head><title>Site Can't Be Reached</title>"
           "<style>"
           "body { font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Helvetica, Arial, sans-serif; background: #f7f9fa; color: #333; display: flex; align-items: center; justify-content: center; height: 100vh; margin: 0; }"
           ".container { max-width: 400px; padding: 40px; background: white; border-radius: 8px; box-shadow: 0 4px 12px rgba(0,0,0,0.08); }"
           "h1 { font-size: 22px; color: #d93025; margin-bottom: 10px; }"
           "p { line-height: 1.6; color: #5f6368; }"
           ".code { font-family: monospace; background: #eee; padding: 2px 4px; border-radius: 4px; }"
           "</style></head><body>"
           "<div class='container'>"
           "<h1>Address Not Found</h1>"
           "<p>We couldn't find the server at <span class='code'>" + host + "</span>.</p>"
           "<p>Check the spelling or try restarting your connection.</p>"
           "</div></body></html>";
}

string ErrorPage::tls_error() {
    return "<html><head><title>Security Error</title>"
           "<style>"
           "body { font-family: sans-serif; background: #fff5f5; color: #c53030; display: flex; align-items: center; justify-content: center; height: 100vh; margin: 0; }"
           ".container { text-align: center; border: 1px solid #feb2b2; padding: 30px; border-radius: 12px; background: white; }"
           "svg { width: 48px; height: 48px; margin-bottom: 15px; fill: #f56565; }"
           "h1 { font-size: 20px; }"
           "</style></head><body>"
           "<div class='container'>"
           "<svg viewBox='0 0 24 24'><path d='M12 2C6.48 2 2 6.48 2 12s4.48 10 10 10 10-4.48 10-10S17.52 2 12 2zm1 15h-2v-2h2v2zm0-4h-2V7h2v6z'/></svg>"
           "<h1>Secure Connection Failed</h1>"
           "<p style='color:#4a5568'>The handshake failed. This might be an expired certificate or an unsupported protocol.</p>"
           "</div></body></html>";
}

string ErrorPage::http_error(int status) {
    string message = (status == 404) ? "Page Not Found" : "Something Went Wrong";
    
    return "<html><head><title>Error " + to_string(status) + "</title>"
           "<style>"
           "body { font-family: 'Inter', system-ui, sans-serif; background: #1a202c; color: white; display: flex; align-items: center; justify-content: center; height: 100vh; margin: 0; }"
           ".container { text-align: center; }"
           "h1 { font-size: 80px; margin: 0; font-weight: 800; opacity: 0.2; }"
           "h2 { font-size: 24px; margin-top: -20px; }"
           "a { color: #63b3ed; text-decoration: none; border-bottom: 1px solid; }"
           "</style></head><body>"
           "<div class='container'>"
           "<h1>" + to_string(status) + "</h1>"
           "<h2>" + message + "</h2>"
           "<p><a href='/'>Return to Safety</a></p>"
           "</div></body></html>";
}