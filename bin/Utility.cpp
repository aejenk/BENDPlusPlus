#include "Utility.h"

namespace Utility {
    std::vector<std::string> split(std::string s, char delimit) {
        std::stringstream ss (s);
        std::string segment;
        std::vector<std::string> seglist;

        while(std::getline(ss, segment, delimit)){
            seglist.push_back(segment);
        }

        return seglist;
    };
}