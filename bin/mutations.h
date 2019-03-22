#include <iostream>
#include <string>
#include <algorithm>
#include <random>

using namespace std;

enum class muts {
    SCATTER, CHUNKS, REPEAT, REVERSE, REMOVE, MOVE, ZERO, SWAP, ISWAP/*, CHUNK0 */
};

class Mutation {
    public:
        size_t chunksize = 1;
        size_t repeats = 1;
        void mutate(muts mutation, size_t safetybuf, int iterations, string& contents);

    private:
        uniform_int_distribution<size_t> dist;
        int iter;
        void mutchunks(string& contents);
        void mutrepeat(string& contents);
        void mutremove(string& contents);
        void mutscatter(string& contents);
        void mutreverse(string& contents);
        void mutmove(string& contents);
        void mutzero(string& contents);
        void mutswap(string& contents);
        void mutiswap(string& contents);
        char randomASCII();
};