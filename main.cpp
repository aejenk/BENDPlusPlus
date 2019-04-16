// #include "bin/binbend.h"
#include "bin/CalBender.h"

using namespace std;


int main() {

    Bender *bb = new CalBender();
    bb->loadFile("voila.mp4");
    bb->addMutation("ChunkM", new ChunkMutation());
    bb->mutateUsing("ChunkM", {
        {"iterations", 1000L},
        {"chunksize", 26000L}
    });
    bb->saveContents();

    return 0;
}