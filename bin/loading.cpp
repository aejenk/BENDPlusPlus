#include <iostream>
#include <iomanip>
#include <string>
#define FIXED_FLOAT(x,y) fixed << setprecision(y) <<(x)
#define CLEARLINE() std::cout << std::setfill(' ') << std::setw(50) << "%\r";

struct LoadingBar {
    std::string name = "";
    size_t steps;
    float progressBuffer = 0.0;
    float progress = 0.0;

    LoadingBar(){}

    LoadingBar(size_t psteps){
        steps = psteps;
    }

    ~LoadingBar(){
        std::cout << std::endl;
    }

    void setLabel(std::string pname){
        name = pname;
    }

    void nextStep(){
        progressBuffer += (1.0/steps);
        if(progressBuffer >= 0.0001){
            progress += progressBuffer;
            progressBuffer = 0.0;
        }
        else return;
        std::cout << "[" << name << "] " << FIXED_FLOAT(progress*100.0,2) << "%\r" ;
        std::cout.flush();
    }
};