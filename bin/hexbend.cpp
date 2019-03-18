#include "hexbend.h"

using namespace std;

// PUBLIC

void HexBender::loadFile(const string& filename){
    contents = toHex(loadFileAsStr(filename));
}

void HexBender::mutate(const int iter){
    unsigned long len = contents.length();

    for(int i = 0; i < iter; i++){
        contents[rand() % len] = hexc[rand() % 16];
    }
}

void HexBender::saveFile(const string& filename){
    ofstream ofile (filename, ios::out | ios::binary);
    ofile << toStr(contents);
}

// PRIVATE

string HexBender::toHex(const string& s){
    ostringstream ret;

    for (string::size_type i = 0; i < s.length(); ++i)
    {
        int z = s[i]&0xff;
        ret << hex << setfill('0') << setw(2) << uppercase << z;
    }

    return ret.str();
}

string HexBender::toStr(const string& s){
    string newString;

    int len = s.length();
    for(int i=0; i< len; i+=2){
        string byte = s.substr(i,2);
        char chr = (char) (int)strtol(byte.c_str(), NULL, 16);
        newString.push_back(chr);
    }

    return newString;
}

string HexBender::loadFileAsStr(const string& filename){
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
