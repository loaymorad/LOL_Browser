#include "http_parser.h"
#include <sstream>

using namespace std;

HttpResponse HttpParser::parse(const std::string& data) {
    HttpResponse res;
    istringstream stream(data);
    string line;

    // status line
    getline(stream, line);
    if(line.find("HTTP/") != string::npos) {
        res.status = stoi(line.substr(9,3));
    }

    // headers
    while(getline(stream, line) && line != "\r"){
        auto colon = line.find(":");
        if(colon != string::npos){
            string key = line.substr(0, colon);
            string value = line.substr(colon + 1);
            
            if(!value.empty() && value[0] == ' ')
                value.erase(0, 1);

            if(!value.empty() && value.back() == '\r')
                value.pop_back();
            
            res.headers[key] = value;
        }
    }

    // body
    string body;
    while(getline(stream, line)){
        body += line + "\n";
    }

    res.body = body;
    
    return res;
}