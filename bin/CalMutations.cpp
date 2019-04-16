#include "Mutation.h"
#include "loading.cpp"
#include <random>

#ifndef BASEMUT_CPP
#define BASEMUT_CPP

struct CalMutation : Mutation{
    std::default_random_engine generator;
    std::uniform_int_distribution<size_t> indexDist;
    size_t safetymin = 0;

    void mutate(std::string &contents, std::map<std::string, std::any> options) override {};
};

struct ChunkMutation : CalMutation{

    void mutate(std::string &contents, std::map<std::string, std::any> options) override{
        long iter = std::any_cast<long>(options.find("iterations"));
        long chunksize = std::any_cast<long>(options.find("chunksize"));

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
    }
};

#endif