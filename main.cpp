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

    Bender *bb = new CalBender();

    bb->addMutation("ThisShouldFail", new WrongMut());

    std::map<std::string, std::any> options = {
        {"iterations", 10},
        {"chunksize", 25000000},
        {"echodelay", 0.10f},
        {"echolength", 2},
        {"raindelay", 1000000},
        {"rainsize", 1},
        {"incrementby", 120},
        {"repeats", 4}
    };

    bb->loadFile("bk.avi");
    bb->loadDefaultMutations();

    std::vector<std::string> modes = {
        "Random", "Move", "Repeat", "Remove",
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