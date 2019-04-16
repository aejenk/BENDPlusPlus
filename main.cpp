// #include "bin/binbend.h"
#include "bin/CalBender.h"

using namespace std;


struct WrongMut : Mutation {
    void mutate(std::string &contents, std::map<std::string, std::any> options){
        return;
    }

    std::string to_string(std::map<std::string, std::any> options){
        return "";
    }
};

int main() {
    std::cout.sync_with_stdio(false);

    CalBender *bb = new CalBender();

    bb->addMutation("ThisShouldFail", new WrongMut());

    std::map<std::string, std::any> options = {
        {"iterations", 100L},
        {"chunksize", 1000L},
        {"echodelay", 0.10},
        {"echolength", 2},
        {"raindelay", 100},
        {"rainsize", 1},
        {"incrementby", 120},
        {"repeats", 4}
    };

    bb->loadFile("MAX300.avi");
    bb->loadDefaultMutations();

    std::vector<std::string> modes = {
        "Random", "Move", "Remove",
        "Reverse", "Null", "Swap", "Increment",
        "Rainbow", "Echo", "Average"
    };

    for(auto mode : modes){
        bb->mutateUsing(mode, options);
        bb->saveContents();
        bb->resetFile();
    }


    return 0;
}