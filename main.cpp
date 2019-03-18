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

void testbin(BinBender bx, string name) {
    // BinBender bx;
    // bx.loadFile("h.avi");
    bx.mutate(600);
    bx.saveFile(name);
}

int main() {
    srand(time(0));

    BinBender bx;
    bx.loadFile("blocks.avi");

    for(int i = 0; i < 4; i++){
        testbin(bx, "blocks" + to_string(i) + ".avi");
    }

    return 0;
}