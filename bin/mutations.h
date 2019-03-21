#include <iostream>
#include <string>
#include <algorithm>
#include <random>

using namespace std;

enum class muts {
    SCATTER, CHUNKS, REPEAT, REVERSE, REMOVE, MOVE, ZERO, SWAP, ISWAP
};

class Mutation {
    public:
        size_t chunksize = 1;
        size_t repeats = 1;
        void mutchunks(uniform_int_distribution<size_t> dist, const int iter, string& contents);
        void mutrepeat(uniform_int_distribution<size_t> dist, const int iter, string& contents);
        void mutremove(uniform_int_distribution<size_t> dist, const int iter, string& contents, int& bufs, int &remBufs);
        void mutscatter(uniform_int_distribution<size_t> dist, const int iter, string& contents);
        void mutreverse(uniform_int_distribution<size_t> dist, const int iter, string& contents);
        void mutmove(uniform_int_distribution<size_t> dist, const int iter, string& contents);
        void mutzero(uniform_int_distribution<size_t> dist, const int iter, string& contents);
        void mutswap(uniform_int_distribution<size_t> dist, const int iter, string& contents);
        void mutiswap(uniform_int_distribution<size_t> dist, const int iter, string& contents);
        char randomASCII();
};