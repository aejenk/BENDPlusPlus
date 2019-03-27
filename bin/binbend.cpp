#include "binbend.h"

using namespace std;

// PUBLIC

// Constructor
BinBender::BinBender(const string& optfile){    
    INIReader options (optfile);

    if (options.ParseError() != 0) {
        std::cout << "Can't find '" << optfile << "'. Please create an options file.\n";
        return;
    }

    // Gets filename from options.
    string fname = options.Get("Bender Options", "filename", "NO FILE");

    // Loads the file into the bender.
    takeTimeWithoutReturn("Loading", loadFile(fname));
    cout << endl;

    // Loads options from .ini file.
    smodes         = options.Get("Bender Options", "modes", "NO MODE");
    mut.rchunksize = parserange(options.Get("Bender Options", "chunksize", "NO CHUNKSIZE"));
    mut.rrepeats   = parserange(options.Get("Bender Options", "repeats", "NO REPEATS"));
    mut.riters     = parserange(options.Get("Bender Options", "iterations", "NO ITERATIONS"));
    loops          = options.GetInteger("Bender Options", "loops", 1);     

    // Parses the mode string as a usable mode variable.
    modes = parsemodes(smodes);
}

void BinBender::start(){
    for(int i = 0; i < loops; i++){
        for(auto mut: modes){
            mutate(mut.first, true); // safely mutates 1000 bytes via SCATTERing
            takeTimeWithoutReturn("SAVING", saveFile());
            reset();
            cout << endl;
        }
    }
}

void BinBender::displayOptions(){
    auto rangestr = [](pair<size_t, size_t> r) -> string {
        if(r.first == r.second) return to_string(r.first);
        else return to_string(r.first) + "-" + to_string(r.second);
    };

    cout << endl;
    cout << setfill('-') << setw(40) << "\n";
    cout << "Bender initalized with parameters:" << endl
         << "Name      : " << filename << "." << extension << endl
         << "Modes     : " << smodes << endl
         << "Chunksize : " << rangestr(mut.rchunksize) << endl
         << "Repeats   : " << rangestr(mut.rrepeats) << endl
         << "Iterations: " << rangestr(mut.riters) << endl
         << "Loops     : " << loops << endl;
    cout << setfill('-') << setw(40) << "\n";
}

void BinBender::loadFile(const string& fname){
    contents = loadFileAsStr(fname);
    if(contents == "") return;
    _mkdir("output");
    extension = fname.substr(fname.rfind('.'));
    filename = fname.substr(0, fname.rfind('.'));
    backup += contents; // backs up file for later restoration
}

void BinBender::mutate(muts type, bool safe /*= false*/){
    mut.initDists();
    mut.mutate(type, (safe) ? safetymin : 0, contents);
}

void BinBender::saveFile(){
    stringstream ss;
    ss << "output/" << filename << mut.getMutString() << extension;
    string savefile = ss.str();

    cout << "Saving [" << savefile << "]";

    // Opens a file with [filename]
    ofstream ofile (savefile, ios::out | ios::binary);
    char buffer[bufferSize];

    int a = 0;

    size_t filesize = contents.size();

    // Buffers contents into file according to amount of buffers used to load file.
    for(int i = 0; (i*bufferSize) < filesize; i++){
        // Used for printing a loading bar.
        a++;
        if(a >= (filesize/(bufferSize*30))){
            cout << ".";
            a = 0;
        }
        // Saves a chunk of bytes into the file.
        string bufferstr = contents.substr(i*bufferSize, bufferSize);
        ofile << bufferstr;
    }
}

// PRIVATE
string BinBender::loadFileAsStr(const string& filename){
    ifstream ifile = ifstream(filename, ios::in | ios::binary | ios::ate);

    string ss; // ? May be replaced with stringstream

    if(ifile.is_open()){
        size_t size = ifile.tellg();
        ifile.seekg(0, ios::beg);

        bufferSize = 1024;

        if(size >= 1024*1024*1024){
            // If the file is over a gigabyte, increase buffersize to a megabyte.
            bufferSize *= 1024;
        }

        unique_ptr<char[]> buffer (new char[bufferSize]);

        int n = (size/bufferSize)/30;
        int a = 0;

        cout << "Loading [" << filename << "]";

        while(ifile){
            // Used for loading bar...
            a++;
            if(a >= n){
                cout << ".";
                a = 0;
            }

            // Reads a chunk of bytes from the file.
            ifile.read(buffer.get(), bufferSize);
            string x = string(buffer.get(), bufferSize);

            ss.append(x); // ? May be replaced with stringstream
        }

        // Sets dumb safety minimum to a 50th of a file.
        // This assumes a heuristic that the file is large enough that -
        //  - the first 50th of the bytes contain the entire header.
        // This heuristic may overcompensate a lot, but it avoids the header consistently.
        safetymin = size / 50;
        return ss;
    }

    cout << "No file loaded - are you sure that file exists?" << endl;
    return "";
}

void BinBender::reset(){
    contents.assign(backup);
    mut.resetMut();
}

vector<string> BinBender::split(string s, char delimit) {
    stringstream ss (s);
    string segment;
    vector<string> seglist;

    while(getline(ss, segment, delimit)){
        seglist.push_back(segment);
    }

    return seglist;
};


pair<size_t, size_t> BinBender::parserange(string range){
    if(range.find('-') == string::npos){
        long rge = stol(range);
        return {rge, rge};
    }
    else{
        vector<string> ranged = split(range, '-');
        return {stol(ranged[0]), stol(ranged[1])};
    }
};

vector<pair<muts, string>> BinBender::parsemodes (string smodes) {
    vector<string> vmodes = split(smodes, ' ');

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