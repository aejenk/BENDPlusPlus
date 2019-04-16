// #include "bin/binbend.h"
#include "bin/CalBender.cpp"

using namespace std;


int main() {

    Bender *bb = new CalBender();
    bb->loadFile("burgyuh.avi");
    bb->addMutation("ChunkM", new ChunkMutation());
    bb->mutateUsing("ChunkM", {
        {"iterations", 1000L},
        {"chunksize", 80000000L}
    });
    bb->saveContents();

    return 0;
}