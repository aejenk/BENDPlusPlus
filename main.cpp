#include "bin/binbend.h"
#include "bin/timetaker.cpp"
#include "lib/inih/INIReader.h"

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

vector<pair<muts, string>> parsemodes (string smodes) {
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
            modes = allmodes;
            break;
        }
    }

    return modes;
}

void bendfile(BinBender bx, string name, muts mutation, int iters) {
    bx.mutate(iters, mutation, true); // safely mutates 1000 bytes via SCATTERing
    takeTimeWithoutReturn("SAVING", bx.saveFile(name));
    cout << endl;
}

int main() {
    string name;
    string smodes;
    int iters;
    int loops;

    string savename;

    BinBender bx;

    INIReader options ("options.ini");

    if (options.ParseError() != 0) {
        std::cout << "Can't find 'options.ini'. Please create one.\n";
        return 1;
    }

    name             = options.Get("Bender Options", "filename", "NO FILE");
    smodes           = options.Get("Bender Options", "modes", "NO MODE");
    bx.mut.chunksize = options.GetInteger("Bender Options", "chunksize", 1);
    bx.mut.repeats   = options.GetInteger("Bender Options", "repeats", 1);
    iters            = options.GetInteger("Bender Options", "iterations", 1);
    loops            = options.GetInteger("Bender Options", "loops", 1); 

    // Manual option input using standard in
    // Kept for legacy, may be removed or used as replacement if no options.ini is found.
    // cout << "Enter filename, chunk size, repeats, iter, and loops: ";
    // cin >> name >> bx.mut.chunksize >> bx.mut.repeats >> iters >> loops;
    // cout << "Enter modes (seperated by space): " ;
    // cin.ignore(256, '\n');
    // std::getline(std::cin, smodes); 
    // cout << endl;

    // Reads options from ini file
    cout << endl;
    cout << setfill('-') << setw(40) << "\n";
    cout << "Bender initalized with parameters:" << endl
         << "Name      : " << name << endl
         << "Modes     : " << smodes << endl
         << "Chunksize : " << bx.mut.chunksize << endl
         << "Repeats   : " << bx.mut.repeats << endl
         << "Iterations: " << iters << endl
         << "Loops     : " << loops << endl;
    cout << setfill('-') << setw(40) << "\n";

    savename.assign(name);
    takeTimeWithoutReturn("LOADING", bx.loadFile(name));
    cout << endl;

    vector<pair<muts, string>> modes = parsemodes(smodes);

    for(int i = 0; i < loops; i++){
        for(auto mut: modes){
            savename.insert(savename.find("."), to_string(i) + mut.second);
            bendfile(bx, savename, mut.first, iters);
            savename.assign(name);
        }
    }

    return 0;
}