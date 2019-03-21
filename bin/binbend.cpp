#include "binbend.h"
#include "timetaker.cpp"

using namespace std;

// PUBLIC

void BinBender::loadFile(const string& filename){
    contents = loadFileAsStr(filename);
    backup += contents; // backs up file for later restoration
}

void BinBender::mutate(const int iter, muts type, bool safe /*= false*/){
    size_t len = contents.length();
    size_t rindex;

    size_t min = (safe) ? safetymin : 0;
    size_t max;

    switch(type){
        case muts::SCATTER :
        case muts::ZERO    : max = len;
        break;
        case muts::CHUNKS  :
        case muts::MOVE    :
        case muts::REMOVE  : 
        case muts::SWAP    :
        case muts::ISWAP   :
        case muts::REVERSE : max = len-mut.chunksize;
        break;
        case muts::REPEAT  : max = len - (mut.chunksize*mut.repeats);
        break;
    }

    uniform_int_distribution<size_t> dist(min,max);

    // switch with time calculations.
    switch(type){
        case muts::SCATTER : takeTimeWithoutReturn("SCT", mut.mutscatter(dist, iter, contents)); break;
        case muts::CHUNKS  :  takeTimeWithoutReturn("CHKS", mut.mutchunks(dist, iter, contents)); break;
        case muts::MOVE    :    takeTimeWithoutReturn("MOV", mut.mutmove(dist, iter, contents)); break;
        case muts::REMOVE  :  takeTimeWithoutReturn("REM", mut.mutremove(dist, iter, contents, bufferings, removedBufs)); break;
        case muts::REVERSE : takeTimeWithoutReturn("REV", mut.mutreverse(dist, iter, contents)); break;
        case muts::REPEAT  :  takeTimeWithoutReturn("REP", mut.mutrepeat(dist, iter, contents)); break;
        case muts::ZERO    :    takeTimeWithoutReturn("ZER", mut.mutzero(dist, iter, contents)); break;
        case muts::SWAP    :    takeTimeWithoutReturn("SWP", mut.mutswap(dist, iter, contents)); break;
        case muts::ISWAP   :   takeTimeWithoutReturn("IWP", mut.mutiswap(dist, iter, contents)); break;
    }

    cout << endl;
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
        if(a >= bufferings/30){
            cout << ".";
            a = 0;
        }

        string bufferstr = contents.substr(i*bufferSize, bufferSize);
        ofile << bufferstr;
    }

    cout << endl;

    // resets contents to backup
    contents.assign(backup);
    bufferings += removedBufs;
    removedBufs = 0;
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
            bufferings++;
            // cout << "TOTAL SIZE... : " << bufferings*bufferSize << endl;
        }

        cout << endl;

        safetymin = size / 50;

        return ss;
    }

    return "";
}