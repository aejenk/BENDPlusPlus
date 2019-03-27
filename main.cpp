#include "bin/binbend.h"

using namespace std;


int main() {
    string optfname = "options.ini";

    BinBender *bx = new BinBender(optfname);
    bx->displayOptions();
    bx->start();

    return 0;
}