// #include "bin/binbend.h"
#include "bin/CalBender.h"

using namespace std;

struct WrongMut : Mutation {
    void mutate(std::string &contents, std::map<std::string, std::any> options){
        return;
    }
};

int main() {

    Bender *bb = new CalBender();

    bb->addMutation("ThisShouldFail", new WrongMut());

    bb->loadFile("voila.mp4");
    bb->addMutation("ChunkM", new ChunkMutation());
    bb->mutateUsing("ChunkM", {
        {"iterations", 1000L},
        {"chunksize", 26000L}
    });
    bb->saveContents();

    return 0;
}