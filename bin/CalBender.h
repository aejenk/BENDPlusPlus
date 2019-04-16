#include "Bender.h"
#include "CalMutations.h"
#include <fstream>
#include <sstream>
#include <direct.h>

#ifndef CALBENDER_H
#define CALBENDER_H

// For cross-platforming.
#if _MSC_VER
    #define mkdir(x) _mkdir(x) 
#endif

class CalBender : public Bender {
    public:
        void loadFile(std::string filename) override;
        void addMutation(std::string mutname, Mutation *m) override;
        void loadDefaultMutations();
        void mutateUsing(std::string mutname, std::map<std::string, std::any> options) override;
        void saveContents() override;
        void resetFile();

    private:
        std::string contents;
        std::string backup;

        size_t bufferSize;
        size_t safetyMin;

        std::string fname;
        std::string extension;
};

#endif