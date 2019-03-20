#include "mutations.h"
#include "timetaker.cpp"

std::default_random_engine generator;

using namespace std;

void Mutation::mutscatter(uniform_int_distribution<size_t> dist, const int iter, string& contents){
    int a = 0;
    size_t rindex;

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

void Mutation::mutchunks(uniform_int_distribution<size_t> dist, const int iter, string& contents){
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

void Mutation::mutrepeat(uniform_int_distribution<size_t> dist, const int iter, string& contents){
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

void Mutation::mutreverse(uniform_int_distribution<size_t> dist, const int iter, string& contents){
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

void Mutation::mutremove(uniform_int_distribution<size_t> dist, const int iter, string& contents, int& bufs, int &remBufs){
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
        bufs--;
        remBufs++;
        dist = uniform_int_distribution<size_t>(min, max);
    }
}

void Mutation::mutmove(uniform_int_distribution<size_t> dist, const int iter, string& contents){
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

void Mutation::mutzero(uniform_int_distribution<size_t> dist, const int iter, string& contents){
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

char Mutation::randomASCII(){
    return static_cast<char>(rand() % 256);
}