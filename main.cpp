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

pair<size_t, size_t> parserange (string range) {
    if(range.find('-') == string::npos){
        long rge = stol(range);
        return {rge, rge};
    }
    else{
        vector<string> ranged = strSplit(range, '-');
        return {stol(ranged[0]), stol(ranged[1])};
    }
}

int main() {
    BinBender bx;
    INIReader options ("options.ini");

    if (options.ParseError() != 0) {
        std::cout << "Can't find 'options.ini'. Please create one.\n";
        return 1;
    }

    string name      = options.Get("Bender Options", "filename", "NO FILE");
    string smodes    = options.Get("Bender Options", "modes", "NO MODE");

    bx.mut.rchunksize = parserange(options.Get("Bender Options", "chunksize", "NO CHUNKSIZE"));
    bx.mut.rrepeats   = parserange(options.Get("Bender Options", "repeats", "NO REPEATS"));
    bx.mut.riters     = parserange(options.Get("Bender Options", "iterations", "NO ITERATIONS"));

    auto rangestr = [](pair<size_t, size_t> r) -> string {
        if(r.first == r.second) return to_string(r.first);
        else return to_string(r.first) + "-" + to_string(r.second);
    };

    long loops       = options.GetInteger("Bender Options", "loops", 1); 

    // Reads options from ini file
    cout << endl;
    cout << setfill('-') << setw(40) << "\n";
    cout << "Bender initalized with parameters:" << endl
         << "Name      : " << name << endl
         << "Modes     : " << smodes << endl
         << "Chunksize : " << rangestr(bx.mut.rchunksize) << endl
         << "Repeats   : " << rangestr(bx.mut.rrepeats) << endl
         << "Iterations: " << rangestr(bx.mut.riters) << endl
         << "Loops     : " << loops << endl;
    cout << setfill('-') << setw(40) << "\n";

    string savename;
    savename.assign(name);
    takeTimeWithoutReturn("LOADING", bx.loadFile(name));
    cout << endl;

    vector<pair<muts, string>> modes = parsemodes(smodes);

    for(int i = 0; i < loops; i++){
        for(auto mut: modes){
            bx.mutate(mut.first, true); // safely mutates 1000 bytes via SCATTERing
            takeTimeWithoutReturn("SAVING", bx.saveFile());
            bx.reset();
            cout << endl;
        }
    }

    return 0;
}