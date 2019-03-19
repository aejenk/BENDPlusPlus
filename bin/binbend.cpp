#include "binbend.h"

using namespace std;

std::default_random_engine generator;

// PUBLIC

void BinBender::loadFile(const string& filename){
    contents = loadFileAsStr(filename);
    backup += contents; // backs up file for later restoration
}

void BinBender::mutate(const int iter, muts type, bool safe /*= false*/){
    size_t len = contents.length();
    size_t randi;

    size_t min = (safe) ? safetymin : 0;
    size_t max;

    switch(type){
        case muts::SCATTER : max = len;
        break;
        case muts::CHUNKS  : 
        case muts::REVERSE : max = len-chunksize;
        break;
        case muts::REPEAT  : max = len - (chunksize*repeats);
        break;
    }

    std::uniform_int_distribution<size_t> distribution(min,max);

    // cout << "Data modified per iteration: " << chunksize*repeats << "bytes" << endl;
    // cout << "Minimum: " << min << ", Maximum: " << max << ", Length: " << len << endl;

    int a = 0;

    // Randomly changes bits all around the file, hence "scattering" the corruption.
    if(type == muts::SCATTER){
        // cout << "Mutating [SCATTER]..." << endl;
        for(int i = 0; i < iter; i++){
            randi = distribution(generator);
            contents[randi] = randomASCII();
        }
    }
    // Just like scatter, except it modifies N bytes every iteration rather than 1 byte.
    else if(type == muts::CHUNKS){
        // cout << "Mutating [CHUNKS]..." << endl;
        for(int i = 0; i < iter; i++){
            randi = distribution(generator);

            for(int j = randi; j < chunksize+randi; j++){
                contents[j] = randomASCII();
            }
        }

    }
    // Repeats random sections of the file N times for every iteration.
    else if(type == muts::REPEAT){
        cout << "Mutating [REPEAT]";
        for(int i = 0; i < iter; i++){
            a++;
            if(a >= iter/15){
                cout << ".";
                a = 0;
            }
            randi = distribution(generator);
            string randomchunk = contents.substr(randi, chunksize);
            
            for(int r = 0; r < repeats && randi+chunksize < len; r++){
                randi += chunksize;
                contents = contents.replace(randi, chunksize, randomchunk);
            }
        }
        cout << endl;
    }
    // Reverses random sections (of size N bytes) of the file M times.
    else if(type == muts::REVERSE){
        // cout << "Mutating [REVERSE]..." << endl;
        for(int i = 0; i < iter; i++){
            randi = distribution(generator);

            string chunk = contents.substr(randi, chunksize);
            reverse(chunk.begin(), chunk.end());
            contents = contents.replace(randi, chunksize, chunk);
        }
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