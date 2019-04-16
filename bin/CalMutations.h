#include "Mutation.h"
#include "loading.cpp"
#include <random>

#ifndef BASEMUT_H
#define BASEMUT_H

struct CalMutation : Mutation {
    std::default_random_engine generator;
    std::uniform_int_distribution<size_t> indexDist;
    size_t safetymin = 0;

    void mutate(std::string &contents, std::map<std::string, std::any> options) override;
};

struct ChunkMutation : CalMutation{
    void mutate(std::string &contents, std::map<std::string, std::any> options) override;
};

#endif
