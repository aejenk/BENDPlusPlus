#include "Bender.h"
#include "CalMutations.cpp"

class CalBender : public Bender{
    public:
        void loadFile(std::string filename) override{
            /* load file using filename */
            contents = "luna's p cool";
        }

        void addMutation(std::string mutname, Mutation *m) override {
            CalMutation *cm = static_cast<CalMutation*>(m);
            cm->safetymin = 10;
            mutations.emplace(mutname, cm);
        }

        void saveContents() override{
            /* basically do nothing */
        }

    private:
        std::string contents;
};