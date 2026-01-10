#pragma once
#include <string>
#include "../network/connection_pool.h"
#include "../network/http.h"

class BrowserController {
public:
    BrowserController();
    void navigate(std::string& raw_url);

private:
    ConnectionPool pool;
    HTTPClient http;
};
