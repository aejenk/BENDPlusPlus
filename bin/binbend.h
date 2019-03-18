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

class BinBender {
    public:
    string contents;
    size_t bufferSize;
    int bufferings;
    void loadFile(const string& filename);
    void mutate(const int iter);
    void saveFile(const string& filename);

    private:
    string loadFileAsStr(const string& filename);
    char randomASCII();
};