#include "binbend.h"
#include "timetaker.cpp"

using namespace std;

// PUBLIC

void BinBender::loadFile(const string& filename){
    contents = loadFileAsStr(filename);
    backup += contents; // backs up file for later restoration
}

void BinBender::mutate(const int iter, muts type, bool safe /*= false*/){
    mut.mutate(type, (safe) ? safetymin : 0, iter, contents);
}

void BinBender::saveFile(const string& filename){
    // cout << "Total bufferings - Bufferings: " << bufferings << ", removedBufs: " << removedBufs << ", size: " << contents.size() << ", total!: " << bufferings*bufferSize << endl;
    cout << "Saving [" << filename << "]";

    // Opens a file with [filename]
    ofstream ofile (filename, ios::out | ios::binary);
    char buffer[bufferSize];

    int a = 0;

    size_t filesize = contents.size();

    // Buffers contents into file according to amount of buffers used to load file.
    for(int i = 0; (i*bufferSize) < filesize; i++){
        a++;
        if(a >= (filesize/(bufferSize*30))){
            cout << ".";
            a = 0;
        }
        string bufferstr = contents.substr(i*bufferSize, bufferSize);
        ofile << bufferstr;
    }

    // cout << endl;

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

        int n = (size/bufferSize)/30;
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
            // cout << "TOTAL SIZE... : " << bufferings*bufferSize << endl;
        }

        // cout << endl;

        safetymin = size / 50;

        return ss;
    }

    return "";
}