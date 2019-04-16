// #include "bin/binbend.h"
#include "bin/CalBender.h"
#include "lib/inih/INIReader.h"

std::vector<std::string> split(std::string s, char delimit) {
    std::stringstream ss (s);
    std::string segment;
    std::vector<std::string> seglist;

    while(std::getline(ss, segment, delimit)){
        seglist.push_back(segment);
    }

    return seglist;
};

int main() {
    std::cout.sync_with_stdio(false);
    INIReader inioptions ("options.ini");

    if(inioptions.ParseError() != 0){
        std::cout << "Please create an option file [options.ini]" << std::endl;
        exit(EXIT_FAILURE);
    }

    CalBender *bb = new CalBender();

    // Main options
    bb->loadFile(inioptions.Get("Main Options", "filename", ""));
    int loops = inioptions.GetInteger("Main Options", "loops", 1);
    std::vector<std::string> modes = split(inioptions.Get("Main Options", "modes", ""), ' ');

    // All Mode options
    std::map<std::string, std::any> options = {
        {"iterations", static_cast<int>(inioptions.GetInteger("CALMode options", "iterations", 0))},
        {"chunksize", static_cast<int>(inioptions.GetInteger("CALMode options", "chunksize", 0))},
        // Repeat options
        {"repeats", static_cast<int>(inioptions.GetInteger("Repeat Options", "repeats", 0))},
        // Increment options
        {"incrementby", static_cast<int>(inioptions.GetInteger("Increment Options", "incrementby", 0))},
        // Rainbow options
        {"rainsize", static_cast<int>(inioptions.GetInteger("Rainbow Options", "rainsize", 0))},
        {"raindelay", static_cast<int>(inioptions.GetInteger("Rainbow Options", "raindelay", 0))},
        // Echo options
        {"echodecay", inioptions.GetReal("Echo Options", "echodecay", 0.0)},
        {"echolength", static_cast<int>(inioptions.GetInteger("Echo Options", "echolength", 0))},
    };

    bb->loadDefaultMutations();
    bb->setGlobalMutationOptions(options);

    for(int i = 0; i < loops; i++){
        std::cout << "Loop [" << i << "]" << std::endl;
        for(auto mode : modes){
            bb->mutateUsing(mode);
            bb->saveContents();
            bb->resetFile();
        }
    }


    return 0;
}