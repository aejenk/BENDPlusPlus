#include <iostream>
#include <string>
#include <algorithm>
#include <random>

using namespace std;

enum class muts {
    SCATTER, CHUNKS, REPEAT, REVERSE, REMOVE, MOVE, ZERO, SWAP, ISWAP/*, CHUNK0 */
};

enum class OPTIONS {
    CHUNKSIZE, REPEATS, ITERS
};

class Mutation {
    public:
        pair<size_t, size_t> rchunksize = {1,1};
        pair<size_t, size_t> rrepeats   = {1,1};
        pair<size_t, size_t> riters     = {1,1}; 

        void mutate(muts mutation, size_t safetybuf, string& contents);

        void initDists();
        string getMutString();
        void resetMut();

    private:
        size_t chunksize;
        size_t repeats;
        size_t iter;

        uniform_int_distribution<size_t> dist;
        uniform_int_distribution<size_t> csize;
        uniform_int_distribution<size_t> reps;
        uniform_int_distribution<size_t> itr;

        string mutstr = "";

        size_t getOption(OPTIONS);

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