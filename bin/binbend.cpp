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
    cout << "Saving [" << filename << "]";

    ofstream ofile (filename, ios::out | ios::binary);
    char buffer[bufferSize];

    int a = 0;

    for(int i = 0; i < bufferings; i++){
        a++;
        if(a >= bufferings/10){
            cout << ".";
            a = 0;
        }

        string bufferstr = contents.substr(i*bufferSize, bufferSize);
        ofile << bufferstr;
    }

    cout << endl;
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

        int n = (size/bufferSize)/10;
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

        return ss;
    }

    return "";
}

char BinBender::randomASCII(){
    return static_cast<char>(rand() % 256);
}