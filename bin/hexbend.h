#include <fstream>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
#include <time.h>

using namespace std;

class HexBender {
    public:
    string contents;
    void loadFile(const string& filename);
    void mutate(const int iter);
    void saveFile(const string& filename);

    private:
    char hexc[17] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    string loadFileAsStr(const string& filename);
    string toHex(const string& s);
    string toStr(const string& s);
};