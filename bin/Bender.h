#include "Mutation.h"

#ifndef BENDER_H
#define BENDER_H

struct Bender {
    virtual void loadFile(std::string filename) = 0;
    virtual void addMutation(std::string mutname, Mutation *m) = 0;
    virtual void loadDefaultMutations() = 0;
    virtual void mutateUsing(std::string mutname, std::map<std::string, std::any> options) = 0;
    virtual void saveContents() = 0;
    virtual void resetFile() = 0;
    std::map<std::string, Mutation*> mutations;
};

#endif