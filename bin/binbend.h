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
    string contents;
    string backup;
    size_t bufferSize;
    size_t safetymin;
    int bufferings;
    void loadFile(const string& filename);
    void mutate(const int iter = 1, muts type, bool safe = false);
    void saveFile(const string& filename);

    private:
    string loadFileAsStr(const string& filename);
    char randomASCII();
};