#include <fstream>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
// #include <cstdlib>
// #include <cstddef>

using namespace std;

char hexc[17] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

char randomHex(){
    return hexc[rand()%16];
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

// format hex code to be spaced out per 2 character (8-bit, 4-bits per hex)
string hexFormat(string& hex)
{
    int len = hex.length();
    int spaces = (len/2)-1;

    cout << "String with length [" << len << "] and spaces [" << spaces << "]" << endl;

    int a = 0;
    int wait = 1000; 

    for(int i = 2; i < len+spaces; i+=3){
        a++;
        if(a > wait){
            cout << ".";
            a = 0;
        }
        hex.insert(i, 1, ' ');
    }

    cout << endl;

    return hex;
}

string loadFileAsString(const string& filename, bool binary){
    ifstream ifile = (binary) ? ifstream(filename, ios::in | ios::binary | ios::ate) : ifstream(filename, ios::in | ios::ate);

    if(ifile.is_open()){
        ifstream::pos_type size = ifile.tellg();
        char * memblock = new char [size];
        ifile.seekg(0, ios::beg);
        ifile.read(memblock, size);
        ifile.close();

        // cout << "File completely loaded in memory." << endl;

        // cout << size;

        return string(memblock, size);
    }

    return "";
}

string mutateHex(string hex, int iterations){
    unsigned long len = hex.length();

    // cout << "Beginning mutations..." << endl;
    for(int i = 0; i < iterations; i++){
        hex[rand() % len] = randomHex();
    }
    // cout << "Mutations complete!" << endl;

    return hex;
}

void hexToFile(string hexcode, const string& filename){
    cout << "Saving hexcode as binary to file [" << filename << "]" << endl;
    ofstream ofile (filename, ios::out | ios::binary);
    ofile << ToStr(hexcode);
}

string fileToHex(const string& filename){
    string hex = ToHex(loadFileAsString(filename, true), true);

    string hexCode;

    for (size_t i = 0; i < hex.size(); i += 2)
    hexCode += hex.substr(i, 2);

    return hexCode;
}

int main() {
    string hexcode = fileToHex("covvv.png");

    string mut;

    int n = 1;

    for(int i = 0; i < n; i++){
        char buf[50];
        mut = mutateHex(hexcode, 1);
        int n = sprintf(buf, "mut-%d.png", i);
        hexToFile(mut, string(buf, n));
    }

    return 0;
}