#include <fstream>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
#include <time.h>
#include <string.h>

using namespace std;

enum class muts {
    SCATTER, CHUNKS, REPEAT, REVERSE
};

class BinBender {
    public:
    int chunksize = 1;
    int repeats = 1;
    void loadFile(const string& filename);
    void mutate(const int iter, muts type, bool safe = false);
    void saveFile(const string& filename);

    private:
    string contents;
    string backup;
    size_t bufferSize;
    size_t safetymin;
    int bufferings;
    string loadFileAsStr(const string& filename);
    char randomASCII();
};