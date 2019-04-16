#include <iostream>
#include <string>
#include <algorithm>
#include <random>
#include <math.h>

#include "loading.cpp"

using namespace std;

enum class muts {
    SCATTER, CHUNKS, REPEAT, REVERSE, REMOVE, MOVE, ZERO,
    SWAP, ISWAP, INCREMENT, RAINBOW, ECHO, AVERAGE
};

enum class OPTIONS {
    CHUNKSIZE, REPEATS, ITERS, INC_BY, RAIN_DELAY, RAIN_SIZE,
    DECAY, PERSIST
};

class Mutation {
    public:
        // Pairs of options
        pair<size_t, size_t> rchunksize = {1,1};
        pair<size_t, size_t> rrepeats   = {1,1};
        pair<size_t, size_t> riters     = {1,1}; 
        pair<size_t, size_t> rincby     = {1,1};
        pair<size_t, size_t> rraindelay = {1,1};
        pair<size_t, size_t> rrainsize  = {1,1};
        pair<float, float> rdecay     = {1.0,1.0};
        pair<size_t, size_t> rpersist   = {1,1};

        // General mutation function with an enum parameter to specify.
        void mutate(muts mutation, size_t safetybuf, string& contents);

        // Initialises the distributions for the options
        void initDists();

        // Retrieves the mutation string
        string getMutString();

        // Resets the mutation.
        void resetMut();

    private:
        // Options
        size_t chunksize;
        size_t repeats;
        size_t iter;
        size_t incby;
        size_t raindelay;
        size_t rainsize;
        float decay;
        size_t persist;

        // Loading bar
        LoadingBar loadingBar;

        // Distributions for specific options.
        uniform_int_distribution<size_t> dist; // used for random indexes
        uniform_int_distribution<size_t> csize;
        uniform_int_distribution<size_t> reps;
        uniform_int_distribution<size_t> itr;
        uniform_int_distribution<size_t> incbyd;
        uniform_int_distribution<size_t> raindelayd;
        uniform_int_distribution<size_t> rainsized;
        uniform_real_distribution<float> decayd;
        uniform_int_distribution<size_t> persistd;

        // A representation of the mutation in string form.
        string mutstr = "";

        // Retrieves an option generator based on the option passed.
        template<typename T=size_t>
        T getOptionGenerator(OPTIONS);

        // Specific mutation functions //
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
        // grabs chunks of bytes and increments them
        void mutincrement(string& contents);
        // grabs chunks of bytes and adds them with an increasing sequence
        void mutrainbow(string& contents);
        // echoes chunks of bytes - can customize delay and persistence.
        void mutecho(string& contents);
        // averages chunks of bytes
        void mutaverage(string& contents);

        // generates a random ASCII character
        char randomASCII();
};