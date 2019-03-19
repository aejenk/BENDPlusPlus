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

void bendfile(BinBender bx, string name, muts mutation) {
    bx.mutate(2, mutation, true); // safely mutates 1000 bytes via SCATTERing
    bx.saveFile(name);
}

int main() {
    srand(time(0));

    cout << "Enter filename: ";
    string name;
    cin >> name;

    cout << endl;

    string savename;
    savename.assign(name);

    BinBender bx;
    bx.loadFile(name);
    bx.chunksize = 3;
    bx.repeats = 2;

    for(int i = 0; i < 20; i++){
        savename.insert(savename.find("."), to_string(i) + "-SCT");
        bendfile(bx, savename, muts::SCATTER);
        savename.assign(name);
        savename.insert(savename.find("."), to_string(i) + "-CHK");
        bendfile(bx, savename, muts::CHUNKS);
        savename.assign(name);
        savename.insert(savename.find("."), to_string(i) + "-REP");
        bendfile(bx, savename, muts::REPEAT);
        savename.assign(name);
        savename.insert(savename.find("."), to_string(i) + "-REV");
        bendfile(bx, savename, muts::REVERSE);
        savename.assign(name);
    }

    return 0;
}