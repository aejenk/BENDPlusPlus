#include <fstream>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
#include "mutations.h"
#include <direct.h>

// For cross-platforming.
#if _MSC_VER
    #define mkdir(x) _mkdir(x) 
#endif

using namespace std;

class BinBender {
    public:
    Mutation mut;
    void loadFile(const string& filename);
    void mutate(muts type, bool safe = false);
    void saveFile();
    void reset();

    private:
    string contents;
    string backup;
    string extension;
    string filename;
    size_t bufferSize;
    size_t safetymin;
    string loadFileAsStr(const string& filename);

    // mutations
    // void mutchunks(uniform_int_distribution<size_t> dist, const int iter);
    // void mutrepeat(uniform_int_distribution<size_t> dist, const int iter);
    // void mutremove(uniform_int_distribution<size_t> dist, const int iter);
    // void mutscatter(uniform_int_distribution<size_t> dist, const int iter);
    // void mutreverse(uniform_int_distribution<size_t> dist, const int iter);
    // void mutmove(uniform_int_distribution<size_t> dist, const int iter);
};