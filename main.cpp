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
    bx.mutate(600);
    bx.saveFile(name);
}

int main() {
    srand(time(0));

    string name = "sacrifgice.avi";
    string savename;
    savename.assign(name);

    BinBender bx;
    bx.loadFile(name);

    for(int i = 0; i < 40; i++){
        savename.insert(savename.find("."), to_string(i));
        testbin(bx, savename);
        savename.assign(name);
    }

    return 0;
}