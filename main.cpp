#include <fstream>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <vector>

using namespace std;

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

// converts a vector of 8-bit hexadecimals into a long string
string ToStr(const vector<string> vs){    
    // Iterate over vs
    stringstream bin;
    for(auto const& hexcode: vs){
        unsigned int x;
        stringstream ss;
        ss << std::hex << hexcode;
        ss >> x;
        bin << static_cast<char>(x);   
    }

    return bin.str();
}

// format hex code to be spaced out per 2 character (8-bit, 4-bits per hex)
string hexFormat(string& hex)
{
    int len = hex.length();
    int spaces = (len/2)-1;

    for(int i = 2; i < len+spaces; i+=3){
        hex.insert(i, 1, ' ');
    }

    return hex;
}

// converts a file into hex
void fileToHex(const string& inname, const string& outname){
    ifstream::pos_type size;
    char * memblock;

    /*  The input file has the flags set to be a binary file.
        The ios::ate flag is to know the size of the file.
        The output file has no flags set as they are not important */
    ifstream ifile (inname, ios::in | ios::binary | ios::ate);
    ofstream ofile (outname);

    if(ifile.is_open()){
        // reads input binary file
        size = ifile.tellg();
        memblock = new char [size];
        ifile.seekg(0, ios::beg);
        ifile.read(memblock, size);
        ifile.close();

        cout << "File completely loaded in memory." << endl;

        cout << size;

        // converts input binary file into hexcode
        string hex = ToHex(string(memblock, size), true);

        // stores formatted hexcode into the output file
        ofile << hexFormat(hex);
    }
}

void hexToFile(const string& inname, const string& outname){
    ofstream::pos_type size;
    char * memblock;
    
    /*  The input file has an ios::ate flag to know its size.
        The output file has the ios::binary flag, since its a binary file. */
    ifstream ifile (inname, ios::in | ios::ate);
    ofstream ofile (outname, ios::out | ios::binary);

    if(ifile.is_open()){
        // reads input file
        size = ifile.tellg();
        memblock = new char [size];
        ifile.seekg(0, ios::beg);
        ifile.read(memblock, size);
        ifile.close();

        cout << "File completely loaded in memory." << endl;

        cout << size;

        // Splits hexcode according to spaces
        string hex = string(memblock, size);
        stringstream ss (hex);
        string segment;
        vector<string> seglist;

        while(getline(ss, segment, ' ')){
            seglist.push_back(segment);
        }

        // stores the binary string back into the output file
        ofile << ToStr(seglist);
    }
}

int main() {
    fileToHex("old.png", "hex.txt"); // converts a binary file (old.png) to a text file storing its hexcode
    hexToFile("hex.txt", "new.png"); // converts a text file with hexcode into a binary file (old.png)
    return 0;
}