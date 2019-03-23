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
        // mutates chunks of bytes
        void mutchunks(string& contents);
        // repeats chunks of bytes
        void mutrepeat(string& contents);
        // removes chunks of bytes
        void mutremove(string& contents);
        // randomly changes bytes
        void mutscatter(string& contents);
        // reverses chunks of bytes
        void mutreverse(string& contents);
        // moves chunks of bytes around
        void mutmove(string& contents);
        // randomly sets bytes to 0
        void mutzero(string& contents);
        // swaps chunks of bytes around
        void mutswap(string& contents);
        // swaps chunks of bytes around - uses iteration - legacy
        void mutiswap(string& contents);
        char randomASCII();
};