#include "binbend.h"

using namespace std;

// PUBLIC

void BinBender::loadFile(const string& filename){
    contents = loadFileAsStr(filename);
}

void BinBender::mutate(const int iter = 1){
    unsigned long len = contents.length();

    for(int i = 0; i < iter; i++){
        contents[rand() % len] = randomASCII();
    }
}

void BinBender::saveFile(const string& filename){
    cout << "SAVING FILE..." << endl;

    ofstream ofile (filename, ios::out | ios::binary);
    char buffer[bufferSize];

    for(int i = 0; i < bufferings; i++){
        cout << "BEGINLOOP" << endl;
        long begin = i*bufferSize;
        long end = (i+1)*bufferSize-1;

        cout << begin << "," << end << endl;

        cout << "COPYING... " << buffer[0] << "," << buffer[bufferSize] << endl;
        size_t len = contents.copy(buffer, begin, end);

        // cout << "TERMINATE..." << endl;
        buffer[bufferSize] = '\0';

        // cout << "CAST..." << endl;
        string bufferstr = string(buffer, bufferSize);

        buffer[0] = '\0';

        // cout << "WRITE..." << endl;
        ofile << bufferstr;
    }

    // ofile << contents;
}

// PRIVATE

string BinBender::loadFileAsStr(const string& filename){
    ifstream ifile = ifstream(filename, ios::in | ios::binary | ios::ate);

    stringstream ss;

    if(ifile.is_open()){
        ifstream::pos_type size = ifile.tellg();
        ifile.seekg(0, ios::beg);

        bufferSize = 1024 * 1024;
        unique_ptr<char[]> buffer (new char[bufferSize]);

        if(size >= 1024*1024*1024){
            bufferSize *= 4096;
        }

        bufferings = size/bufferSize;
        cout << "Amount of buffering... (" << bufferings << ")" << endl;

        int n = size/(10*bufferSize);
        int a = 0;

        cout << "Starting loop!" << endl;
        while(ifile){
            a++;
            if(a >= n){
                cout << ".";
                a = 0;
            }

            ifile.read(buffer.get(), bufferSize);
            ss << string(buffer.get(), bufferSize);
        }

        cout << endl;
        cout << "Finished loop!" << endl;

        return ss.str();
    }

    return "";
}

char BinBender::randomASCII(){
    return static_cast<char>(rand() % 256);
}