#include "CalMutations.h"

void CalMutation::mutate(string &contents, std::map<std::string, std::any> options) {};

void ChunkMutation::mutate(std::string &contents, std::map<std::string, std::any> options) {
    long iter = std::any_cast<long>(options["iterations"]);
    long chunksize = std::any_cast<long>(options["chunksize"]);

    indexDist = std::uniform_int_distribution<size_t>(safetymin, contents.size()-chunksize);

    LoadingBar *lb = new LoadingBar(iter);
    lb->setLabel("Chunks");

    size_t rindex;

    for(int i = 0; i < iter; i++){
        rindex = indexDist(generator);

        for(int j = rindex; j < chunksize + rindex; j++){
            contents[j] = static_cast<char>(rand() % 256);
        }

        lb->nextStep();
    }

    CLEARLINE();
}