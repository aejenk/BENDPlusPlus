#include <iostream>
#include <iomanip>
#include <string>
#define FIXED_FLOAT(x,y) fixed << setprecision(y) <<(x)

using namespace std;

struct LoadingBar {
    string name = "";
    size_t steps;
    int width;
    float progressBuffer = 0.0;
    float progress = 0.0;

    LoadingBar(){}

    LoadingBar(size_t psteps, int pwidth){
        steps = psteps;
        width = pwidth;
    }

    void setLabel(string pname){
        name = pname;
    }

    void nextStep(){
        progressBuffer += (1.0/steps);
        if(progressBuffer >= 0.001){
            progress += progressBuffer;
            progressBuffer = 0.0;
        }
        else return;
        cout << "[" << name << "](" << FIXED_FLOAT(progress*100.0,1) << ") %\r" ;
        cout.flush();
    }
};