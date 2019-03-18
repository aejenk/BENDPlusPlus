#include "bin/binbend.h"
#include "bin/hexbend.h"

using namespace std;

vector<string> strSplit(string s, char delimit){
    stringstream ss (s);
    string segment;
    vector<string> seglist;

    while(getline(ss, segment, delimit)){
        seglist.push_back(segment);
    }

    return seglist;
}

void testhex() {
    HexBender hx;
    hx.loadFile("skele.jpg");
    hx.mutate(10);
    hx.saveFile("skele-hex.jpg");
}

void testbin() {
    BinBender bx;
    bx.loadFile("skele.jpg");
    bx.mutate(10);
    bx.saveFile("skele-bin.jpg");
}

int main() {
    srand(time(0));

    testhex();
    testbin();

    return 0;
}