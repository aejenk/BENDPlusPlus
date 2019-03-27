#include "binbend.h"
#include "timetaker.cpp"

using namespace std;

// PUBLIC

// Loads file into the Bender
void BinBender::loadFile(const string& fname){
    contents = loadFileAsStr(fname);
    if(contents == "") exit(EXIT_FAILURE);
    _mkdir("output");
    extension = fname.substr(fname.rfind('.'));
    filename = fname.substr(0, fname.rfind('.'));
    backup += contents; // backs up file for later restoration
}

// Mutates a string [iter] times using mode [type], with possible safety.
// Acts as interface to call the mutate function from a Mutate object.
void BinBender::mutate(muts type, bool safe /*= false*/){
    mut.initDists();
    mut.mutate(type, (safe) ? safetymin : 0, contents);
}

// Saves file with [filename].
// Does this by buffering the contents into said file manually - for a speed increase.
void BinBender::saveFile(){
    string savefile = "output/";
    savefile += filename;
    savefile += mut.getMutString() + extension;
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

// Loads a file as a string - no matter if it's binary.
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