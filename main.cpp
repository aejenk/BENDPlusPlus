// #include "bin/binbend.h"
#include "bin/CalBender.cpp"

using namespace std;


int main() {

    Bender *bb = new CalBender();
    bb->loadFile("fakefile.txt");
    bb->addMutation("ChunkM", new ChunkMutation());

    return 0;
}