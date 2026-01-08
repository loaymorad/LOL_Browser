#include "cli.h"
#include <iostream>

using namespace std;

string CLI::get_url(int argc, char *argv[]) {
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <url>" << endl;
        exit(1);
    }
    return argv[1];
}