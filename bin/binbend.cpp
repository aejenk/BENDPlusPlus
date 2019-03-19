#include "binbend.h"

using namespace std;

// PUBLIC

void BinBender::loadFile(const string& filename){
    contents = loadFileAsStr(filename);
    backup += contents; // backs up file for later restoration
}

void BinBender::mutate(const int iter, muts type, bool safe = false){
    unsigned long len = contents.length();

    // Randomly changes bits all around the file, hence "scattering" the corruption.
    if(type == muts::SCATTER){
        for(int i = 0; i < iter; i++){
            // If safe, offset random index to avoid header data.
            // If unsafe, simply pick random index.
            size_t randi = (safe) ? (rand() % (len-safetymin)) + safetymin : (rand() % len);
            contents[randi] = randomASCII();
        }
    }
    // Just like scatter, except it modifies N bytes every iteration rather than 1 byte.
    else if(type == muts::CHUNKS){

    }
    // Repeats random sections of the file N times for every iteration.
    else if(type == muts::REPEAT){

    }
    // Reverses random sections (of size N bytes) of the file M times.
    else if(type == muts::REVERSE){

    }
}

void BinBender::saveFile(const string& filename){
    cout << "Saving [" << filename << "]";

    // Opens a file with [filename]
    ofstream ofile (filename, ios::out | ios::binary);
    char buffer[bufferSize];

    int a = 0;

    // Buffers contents into file according to amount of buffers used to load file.
    for(int i = 0; i < bufferings; i++){
        a++;
        if(a >= bufferings/15){
            cout << ".";
            a = 0;
        }

        string bufferstr = contents.substr(i*bufferSize, bufferSize);
        ofile << bufferstr;
    }

    cout << endl;

    // resets contents to backup
    contents.assign(backup);
}

// PRIVATE

string BinBender::loadFileAsStr(const string& filename){
    ifstream ifile = ifstream(filename, ios::in | ios::binary | ios::ate);

    // stringstream ss;
    string ss;

    if(ifile.is_open()){
        size_t size = ifile.tellg();
        ifile.seekg(0, ios::beg);

        bufferSize = 1024;

        if(size >= 1024*1024*1024){
            bufferSize *= 1024;
        }

        unique_ptr<char[]> buffer (new char[bufferSize]);

        bufferings = 0;

        int n = (size/bufferSize)/15;
        int a = 0;

        cout << "Loading [" << filename << "]";

        while(ifile){
            a++;
            if(a >= n){
                cout << ".";
                a = 0;
            }

            ifile.read(buffer.get(), bufferSize);
            string x = string(buffer.get(), bufferSize);

            // ss << x;
            ss.append(x);
            bufferings++;
            // cout << "TOTAL SIZE... : " << bufferings*bufferSize << endl;
        }

        cout << endl;

        safetymin = size / 50;

        return ss;
    }

    return "";
}

char BinBender::randomASCII(){
    return static_cast<char>(rand() % 256);
}