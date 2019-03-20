#include <fstream>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
#include <time.h>
#include <random>

using namespace std;

enum class muts {
    SCATTER, CHUNKS, REPEAT, REVERSE, REMOVE, MOVE
};

class BinBender {
    public:
    size_t chunksize = 1;
    size_t repeats = 1;
    void loadFile(const string& filename);
    void mutate(const int iter, muts type, bool safe = false);
    void saveFile(const string& filename);

    private:
    string contents;
    string backup;
    size_t bufferSize;
    size_t safetymin;
    int bufferings;
    int removedBufs = 0;
    string loadFileAsStr(const string& filename);
    char randomASCII();
};