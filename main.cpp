#include "ui/cli.h"
#include "browser/controller.h"

using namespace std;

int main(int argc, char *argv[]) {
    CLI cli;
    BrowserController browser;

    string url = cli.get_url(argc, argv);

    browser.navigate(url);

    return 0;
}