#include "mutations.h"
#include "timetaker.cpp"

std::default_random_engine generator;

using namespace std;

void Mutation::initDists() {
    csize = uniform_int_distribution<size_t>(rchunksize.first, rchunksize.second);
    reps = uniform_int_distribution<size_t>(rrepeats.first, rrepeats.second);
    itr = uniform_int_distribution<size_t>(riters.first, riters.second);
}

size_t Mutation::getOption(OPTIONS opt){
    switch(opt){
        case OPTIONS::CHUNKSIZE :
            return (rchunksize.first == rchunksize.second) ? rchunksize.first : csize(generator);
        case OPTIONS::REPEATS :
            return (rrepeats.first == rrepeats.second) ? rrepeats.first : reps(generator);
        case OPTIONS::ITERS :
            return (riters.first == riters.second) ? riters.first : itr(generator);
        default :
            return 0;
    }
}

void Mutation::mutate(muts mutation, size_t safetybuf, string& contents){
    size_t len = contents.length();
    size_t rindex;

    size_t min = safetybuf;
    size_t max;

    chunksize = getOption(OPTIONS::CHUNKSIZE);
    repeats = getOption(OPTIONS::REPEATS);
    iter = getOption(OPTIONS::ITERS);

    switch(mutation){
        case muts::SCATTER :
        case muts::ZERO    : max = len; break;
        case muts::CHUNKS  :
        case muts::MOVE    :
        case muts::REMOVE  :
        case muts::SWAP    :
        case muts::ISWAP   :
        case muts::REVERSE : max = len-chunksize; break;
        case muts::REPEAT  : max = len - (chunksize*repeats); break;
    }

    dist = uniform_int_distribution<size_t>(min,max);

       // switch with time calculations.
    switch(mutation){
        case muts::SCATTER :  takeTimeWithoutReturn("SCT", mutscatter(contents)); break;
        case muts::CHUNKS  :  takeTimeWithoutReturn("CHKS", mutchunks(contents)); break;
        case muts::MOVE    :     takeTimeWithoutReturn("MOV", mutmove(contents)); break;
        case muts::REMOVE  :   takeTimeWithoutReturn("REM", mutremove(contents)); break;
        case muts::REVERSE :  takeTimeWithoutReturn("REV", mutreverse(contents)); break;
        case muts::REPEAT  :   takeTimeWithoutReturn("REP", mutrepeat(contents)); break;
        case muts::ZERO    :     takeTimeWithoutReturn("ZER", mutzero(contents)); break;
        case muts::SWAP    :     takeTimeWithoutReturn("SWP", mutswap(contents)); break;
        case muts::ISWAP   :    takeTimeWithoutReturn("IWP", mutiswap(contents)); break;
    }

    cout << endl;
}

void Mutation::mutscatter(string& contents){
    int a = 0;
    size_t rindex;

    mutstr += "-SCT-i=" + to_string(iter);

    cout << "Mutating [SCATTER]";
        
    for(int i = 0; i < iter; i++){
        a++;
        if(a >= iter/30){
            cout << ".";
            a = 0;
        }

        rindex = dist(generator);
        contents[rindex] = randomASCII();
    }
}

void Mutation::mutchunks(string& contents){
    mutstr += "-CHK-i=" + to_string(iter) + "-c=" + to_string(chunksize);
    int a = 0;
    size_t rindex;

    cout << "Mutating [CHUNKS]";
    for(int i = 0; i < iter; i++){
        a++;

        if(a >= iter/30){
            cout << ".";
            a = 0;
        }

        rindex = dist(generator);

        for(int j = rindex; j < chunksize+rindex; j++){
            contents[j] = randomASCII();
        }
    }
}

void Mutation::mutrepeat(string& contents){
    mutstr += "-REP-i=" + to_string(iter) + "-c=" + to_string(chunksize) + "-r=" + to_string(repeats);

    int a = 0;
    int len = contents.length();
    size_t rindex;

    cout << "Mutating [REPEAT]";
    for(int i = 0; i < iter; i++){
        a++;
        if(a >= iter/30){
            cout << ".";
            a = 0;
        }
        rindex = dist(generator);
        string randomchunk = contents.substr(rindex, chunksize);
        
        for(int r = 0; r < repeats && rindex+chunksize < len; r++){
            rindex += chunksize;
            contents = contents.replace(rindex, chunksize, randomchunk);
        }
    }
}

void Mutation::mutreverse(string& contents){
    mutstr += "-REV-i=" + to_string(iter) + "-c=" + to_string(chunksize);
    int a = 0;
    size_t rindex;

    cout << "Mutating [REVERSE]";
    for(int i = 0; i < iter; i++){
        a++;
        if(a >= iter/30){
            cout << ".";
            a = 0;
        }

        rindex = dist(generator);

        string chunk = contents.substr(rindex, chunksize);
        reverse(chunk.begin(), chunk.end());
        contents = contents.replace(rindex, chunksize, chunk);
    }
}

void Mutation::mutremove(string& contents){
    mutstr += "-REM-i=" + to_string(iter) + "-c=" + to_string(chunksize);
    int a = 0;
    size_t rindex;

    size_t min = dist.min();
    size_t max = dist.max();

    cout << "Mutating [REMOVE]";
    for(int i = 0; i < iter; i++){
        a++;
        if(a >= iter/30){
            cout << ".";
            a = 0;
        }

        rindex = dist(generator);
        contents.erase(rindex, chunksize);

        max -= chunksize;
        dist = uniform_int_distribution<size_t>(min, max);
    }
}

void Mutation::mutmove(string& contents){
    mutstr += "-MOV-i=" + to_string(iter) + "-c=" + to_string(chunksize);
    int a = 0;
    size_t rindex;

    cout << "Mutating [MOVE]";
    for(int i = 0; i < iter; i++){
        a++;
        if(a >= iter/30){
            cout << ".";
            a = 0;
        }

        rindex = dist(generator);

        string sub = contents.substr(rindex, chunksize);
        contents.erase(rindex, chunksize);

        rindex = dist(generator);
        contents.insert(rindex, sub);
    }
}

void Mutation::mutiswap(string& contents){
    mutstr += "-ISW-i=" + to_string(iter) + "-c=" + to_string(chunksize);
    int a = 0;
    size_t rbegin;
    size_t rbegin2;
    size_t rend;

    cout << "Mutating [ISWAP]";
    for(int i = 0; i < iter; i++){
        a++;
        if(a >= iter/30){
            cout << ".";
            a = 0;
        }

        rbegin = dist(generator);
        rbegin2 = dist(generator);
        rend = rbegin + chunksize;

        swap_ranges(contents.begin()+rbegin, contents.begin()+rend, contents.begin()+rbegin2);
    }
}

void Mutation::mutzero(string& contents){
    mutstr += "-ZER-i=" + to_string(iter);
    int a = 0;
    size_t rindex;

    cout << "Mutating [ZERO]";
        
    for(int i = 0; i < iter; i++){
        a++;
        if(a >= iter/30){
            cout << ".";
            a = 0;
        }

        rindex = dist(generator);
        contents[rindex] = 0x00;
    }
}

void Mutation::mutswap(string& contents){
    mutstr += "-SWP-i=" + to_string(iter) + "-c=" + to_string(chunksize);
    int a = 0;
    size_t rindex1;
    size_t rindex2;
    
    cout << "Mutating [SWAP]";

    for(int i = 0; i < iter; i++){
        a++;
        if(a >= iter/30){
            cout << ".";
            a = 0;
        }

        rindex1 = dist(generator);
        rindex2 = dist(generator);
        string sub1 = contents.substr(rindex1, chunksize);
        string sub2 = contents.substr(rindex2, chunksize); 

        contents.replace(rindex1, chunksize, sub2);
        contents.replace(rindex2, chunksize, sub1);
    }
}

char Mutation::randomASCII(){
    return static_cast<char>(rand() % 256);
}

void Mutation::resetMut(){
    mutstr = "";
}

string Mutation::getMutString(){
    return mutstr;
}