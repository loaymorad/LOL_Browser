#include "url.h"
using namespace std;


URL::URL(const string& raw) : raw_(raw) {
    parse();
}


void URL::parse() {
    scheme_ = "http";
    port_ = 80;
    path_ = "/";
    
    auto pos = raw_.find("://");
    if (pos != string::npos) {
        scheme_ = raw_.substr(0, pos);
        raw_ = raw_.substr(pos + 3);
    }

    if (scheme_ == "https") {
        port_ = 443;
    }

    auto path_pos = raw_.find('/');
    if (path_pos != string::npos) {
        host_ = raw_.substr(0, path_pos);
        path_ = raw_.substr(path_pos);
    } else {
        host_ = raw_;
        path_ = "/";
    }
}

std::string URL::scheme() const {
    return scheme_;
}

std::string URL::host() const {
    return host_;
}

std::string URL::path() const {
    return path_;
}

int URL::port() const {
    return port_;
}

std::string URL::to_string() const {
    return scheme_ + "://" + host_ + ":" + std::to_string(port_) + path_;
}

