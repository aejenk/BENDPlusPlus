#include <fstream>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
#include <time.h>

using namespace std;

class BinBender {
    public:
    string contents;
    void loadFile(const string& filename);
    void mutate(const int iter);
    void saveFile(const string& filename);

    private:
    string loadFileAsStr(const string& filename);
    char randomASCII();
};