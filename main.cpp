#include "bin/binbend.h"

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

    cin >> name >> bx.mut.chunksize >> bx.mut.repeats >> iters >> loops;

    cout << endl;

    savename.assign(name);
    bx.loadFile(name);

    // bx.chunksize = 100;
    // bx.repeats = 5;

    vector<pair<muts, string>> allmuts = 
        {
            // {muts::CHUNKS, "-CHK"},
            {muts::MOVE, "-MOV"},
            {muts::REMOVE, "-RMV"},
            // {muts::REPEAT, "-REP"},
            // {muts::REVERSE, "-REV"},
            // {muts::SCATTER, "-SCT"}
        };

    for(int i = 0; i < loops; i++){
        for(auto mut: allmuts){
            savename.insert(savename.find("."), to_string(i) + mut.second);
            bendfile(bx, savename, mut.first, iters);
            savename.assign(name);
        }
    }

    return 0;
}