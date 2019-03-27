#include <fstream>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
#include "mutations.h"
#include "timetaker.cpp"
#include "../lib/inih/INIReader.h"
#include <direct.h>

// For cross-platforming.
#if _MSC_VER
    #define mkdir(x) _mkdir(x) 
#endif

using namespace std;

class BinBender {
    public:
        // Reads options from .ini file, and initialises the bender accordingly.
        BinBender(const string& optfile = "options.ini");
        ~BinBender();

        // Starts a usual bender routine.
        // What this does is run the databending cycle [loop] times.
        // Each loop has a mutation, a save, and a reset.
        // More complicated databending routines can be made manually.
        void start();

        /**
         * Loads a file into the bender.
         * Even though the constructor loads a file from the .ini file,
         * there is planned to be an alternate constructor in the case of where
         * an .ini file does not exist, or its use is not desired.
         * In addition, more files can be loaded later.
         * As such, it stays a public function.
        */
        void loadFile(const string& fname);

        /**
         * These are active functions.
         * Technically, they are all run by the constructor and the start() function.
         * However, there may be an alternate constructor, and the user might
         * want to build their own routine. Hence, these functions remain public.
         */
        void mutate(muts type, bool safe = false); // mutates contents of file
        void saveFile(); // saves the file via buffering using an automated savename
        void reset(); // resets bender to inital settings with the same file
        void displayOptions(); // displays the options that were loaded

    private:
        Mutation mut;
        long loops = 1; // amount of loops to do;
        string contents; // stores contents of file
        string backup; // backs the contents up
        string extension; // the extension of the filename
        string filename; // the actual name of the file
        string smodes; // modes in string form
        vector<pair<muts, string>> modes; // modes in actual form
        size_t bufferSize; // the size of the buffer set
        size_t safetymin; // a heuristic to avoid the header
        string loadFileAsStr(const string& filename); // loads file as string, even if binary

        /**
         * Utility functions.
         * These are not necessarily databending related, but they help with the 
         * organisation of the code.
         */
        vector<string> split(string s, char delimit); // Splits string using a delimiter.
        pair<size_t, size_t> parserange(string range); // Parses a range string. (x-y)
        vector<pair<muts, string>> parsemodes (string smodes); // Parses modes from mode string.

        // Contains all the modes, along with their codes.
        const vector<pair<muts, string>> allmodes = 
            {
                {muts::CHUNKS, "-CHK"},
                {muts::ZERO, "-ZER"},
                {muts::REPEAT, "-REP"},
                {muts::REVERSE, "-REV"},
                {muts::SCATTER, "-SCT"},
                {muts::SWAP, "-SWP"},
                {muts::ISWAP, "-IWP"},
                {muts::MOVE, "-MOV"},
                {muts::REMOVE, "-RMV"},
            };
};