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
    ofstream ofile (filename, ios::out | ios::binary);
    ofile << contents;
}

// PRIVATE

string BinBender::loadFileAsStr(const string& filename){
    ifstream ifile = ifstream(filename, ios::in | ios::binary | ios::ate);

    if(ifile.is_open()){
        ifstream::pos_type size = ifile.tellg();
        char * memblock = new char [size];
        ifile.seekg(0, ios::beg);
        ifile.read(memblock, size);
        ifile.close();

        return string(memblock, size);
    }

    return "";
}

char BinBender::randomASCII(){
    return static_cast<char>(rand() % 256);
}