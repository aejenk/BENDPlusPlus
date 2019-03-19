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

void bendfile(BinBender bx, string name, muts mutation, int iters) {
    bx.mutate(iters, mutation, true); // safely mutates 1000 bytes via SCATTERing
    bx.saveFile(name);
}

int main() {
    srand(time(0));

    cout << "Enter filename, chunk size, repeats, iter, and loops: ";
    string name;
    int iters;
    int loops;

    string savename;

    BinBender bx;

    cin >> name >> bx.chunksize >> bx.repeats >> iters >> loops;

    cout << endl;

    savename.assign(name);
    bx.loadFile(name);

    // bx.chunksize = 100;
    // bx.repeats = 5;

    for(int i = 0; i < loops; i++){
        savename.insert(savename.find("."), to_string(i) + "-SCT");
        bendfile(bx, savename, muts::SCATTER, iters);
        savename.assign(name);
        savename.insert(savename.find("."), to_string(i) + "-CHK");
        bendfile(bx, savename, muts::CHUNKS, iters);
        savename.assign(name);
        savename.insert(savename.find("."), to_string(i) + "-REV");
        bendfile(bx, savename, muts::REVERSE, iters);
        savename.assign(name);
        savename.insert(savename.find("."), to_string(i) + "-REP");
        bendfile(bx, savename, muts::REPEAT, iters);
        savename.assign(name);
    }

    return 0;
}