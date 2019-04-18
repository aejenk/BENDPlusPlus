// #include "bin/binbend.h"
#include "bin/CalBender.h"
#include "lib/inih/INIReader.h"

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
    std::vector<std::string> modes = Utility::split(inioptions.Get("Main Options", "modes", ""), ' ');

    // All Mode options
    std::map<std::string, std::string> options = {
        {"iterations", inioptions.Get("CALMode options", "iterations", "")},
        {"chunksize", inioptions.Get("CALMode options", "chunksize", "")},
        // Repeat options
        {"repeats", inioptions.Get("Repeat Options", "repeats", "")},
        // Increment options
        {"incrementby", inioptions.Get("Increment Options", "incrementby", "")},
        // Rainbow options
        {"rainsize", inioptions.Get("Rainbow Options", "rainsize", "")},
        {"raindelay", inioptions.Get("Rainbow Options", "raindelay", "")},
        // Echo options
        {"echodecay", inioptions.Get("Echo Options", "echodecay", "")},
        {"echolength", inioptions.Get("Echo Options", "echolength", "")},
        // Magnifi options
        {"magnify", inioptions.Get("Magnifi Options", "magnifyby", "")},
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