#include <fstream>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
#include <time.h>
// #include <cstdlib>
// #include <cstddef>

using namespace std;

char hexc[17] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

char randomHex(){
    return hexc[rand()%16];
}

char randomASCII(){
    return static_cast<char>(rand() % 256);
}

vector<string> strSplit(string s, char delimit){
    stringstream ss (s);
    string segment;
    vector<string> seglist;

    while(getline(ss, segment, delimit)){
        seglist.push_back(segment);
    }

    return seglist;
}

// converts a string into hexadecimal characters.
string ToHex(const string& s, bool upper_case)
{
    ostringstream ret;

    for (string::size_type i = 0; i < s.length(); ++i)
    {
        int z = s[i]&0xff;
        ret << std::hex << std::setfill('0') << std::setw(2) << (upper_case ? std::uppercase : std::nouppercase) << z;
    }

    return ret.str();
}

string ToStr(const string hex){
    string newString;

    // cout << "converting hexstr to binstr" << endl;
    int len = hex.length();
    for(int i=0; i< len; i+=2){
        string byte = hex.substr(i,2);
        char chr = (char) (int)strtol(byte.c_str(), NULL, 16);
        newString.push_back(chr);
    }

    return newString;
}

string loadFileAsString(const string& filename, bool binary){
    ifstream ifile = (binary) ? ifstream(filename, ios::in | ios::binary | ios::ate) : ifstream(filename, ios::in | ios::ate);

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

string mutateHex(string hex, int iterations){
    unsigned long len = hex.length();

    for(int i = 0; i < iterations; i++){
        hex[rand() % len] = randomHex();
    }

    return hex;
}

string mutate(string bin, int iterations){
    unsigned long len = bin.length();

    for(int i = 0; i < iterations; i++){
        bin[rand() % len] = randomASCII();
    }

    return bin;
}

void hexToFile(string hexcode, const string& filename){
    cout << "Saving hexcode as binary to file [" << filename << "]" << endl;
    ofstream ofile (filename, ios::out | ios::binary);
    ofile << ToStr(hexcode);
}

void binToFile(string bin, const string& filename){
    cout << "Saving binary to file [" << filename << "]" << endl;
    ofstream ofile (filename, ios::out | ios::binary);
    ofile << bin;
}

string fileToHex(const string& filename){
    return ToHex(loadFileAsString(filename, true), true);
}

void testhex() {
    string hexcode = fileToHex("covvv.png");

    string mut;

    int n = 20;

    for(int i = 0; i < n; i++){
        char buf[50];
        mut = mutateHex(hexcode, 1);
        int m = sprintf(buf, "muthex-%d.png", i);
        hexToFile(mut, string(buf, m));
    }
}

void testbin() {
    string bin = loadFileAsString("H.avi", true);
    string mut;
    int n = 10;

    for(int i = 0; i < n; i++){
        char buf[50];
        mut = mutate(bin, 30);
        int m = sprintf(buf, "mutbin-%d.avi", i);
        binToFile(mut, string(buf, m));
    }
}

int main() {
    srand(time(0));

    // testhex();
    testbin();

    return 0;
}