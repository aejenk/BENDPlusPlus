#include "bin/binbend.h"
#include "bin/timetaker.cpp"

using namespace std;

vector<pair<muts, string>> allmodes = 
        {
            {muts::CHUNKS, "-CHK"},
            {muts::ZERO, "-ZER"},
            {muts::REPEAT, "-REP"},
            {muts::REVERSE, "-REV"},
            {muts::SCATTER, "-SCT"},
            {muts::SWAP, "-SWP"},
            {muts::ISWAP, "-IWP"},
            {muts::MOVE, "-MOV"},
            {muts::REMOVE, "-RMV"},
        };

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
    takeTimeWithoutReturn("SAVING", bx.saveFile(name));
    cout << endl;
}

int main() {
    srand(time(0));

    cout << "Enter filename, chunk size, repeats, iter, and loops: ";
    string name;
    string smodes;
    int iters;
    int loops;

    string savename;

    BinBender bx;

    cin >> name >> bx.mut.chunksize >> bx.mut.repeats >> iters >> loops;
    cout << "Enter modes (seperated by space): " ;
    cin.ignore(256, '\n');
    std::getline(std::cin, smodes); 
    cout << endl;

    savename.assign(name);
    takeTimeWithoutReturn("LOADING", bx.loadFile(name));
    cout << endl;

    vector<string> vmodes = strSplit(smodes, ' ');

    vector<pair<muts, string>> modes;

    for(string mode: vmodes){
        if(mode == "CHUNK") modes.push_back({muts::CHUNKS, "-CHK"});
        else if(mode == "ZERO") modes.push_back({muts::ZERO, "-ZER"});
        else if(mode == "REPEAT") modes.push_back({muts::REPEAT, "-REP"});
        else if(mode == "REVERSE") modes.push_back({muts::REVERSE, "-REV"});
        else if(mode == "SCATTER") modes.push_back({muts::SCATTER, "-SCT"});
        else if(mode == "SWAP") modes.push_back({muts::SWAP, "-SWP"});
        else if(mode == "ISWAP") modes.push_back({muts::ISWAP, "-ISP"});
        else if(mode == "MOVE") modes.push_back({muts::MOVE, "-MOV"});
        else if(mode == "REMOVE") modes.push_back({muts::REMOVE, "-RMV"});
        else if(mode == "ALL"){
            modes.clear();
            modes = allmodes;
            break;
        }
    }

    for(int i = 0; i < loops; i++){
        for(auto mut: modes){
            savename.insert(savename.find("."), to_string(i) + mut.second);
            bendfile(bx, savename, mut.first, iters);
            savename.assign(name);
        }
    }

    return 0;
}