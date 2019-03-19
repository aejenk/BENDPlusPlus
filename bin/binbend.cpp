#include "binbend.h"

using namespace std;

// PUBLIC

void BinBender::loadFile(const string& filename){
    contents = loadFileAsStr(filename);
    backup += contents;
}

void BinBender::mutate(const int iter = 1, muts type = muts::SCATTER){

    // Randomly changes bits all around the file, hence "scattering" the corruption.
    if(type == muts::SCATTER){
        unsigned long len = contents.length();

        for(int i = 0; i < iter; i++){
            size_t randi = (rand() % (len-safetymin));
            randi += safetymin;
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

    ofstream ofile (filename, ios::out | ios::binary);
    char buffer[bufferSize];

    int a = 0;

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