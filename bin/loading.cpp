#include <iostream>
#include <iomanip>
#include <string>
#define FIXED_FLOAT(x,y) fixed << setprecision(y) <<(x)

using namespace std;

struct LoadingBar {
    string name = "";
    size_t steps;
    int width;
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
        progress += (1.0/steps);

        int barWidth = 70;

        cout << "[(" << name << ")";
        int pos = barWidth * progress;
        for (int i = 0; i < barWidth; ++i) {
            if (i < pos) cout << "=";
            else if (i == pos) cout << ">";
            else cout << " ";
        }
        cout << "] " << FIXED_FLOAT(progress * 100.0, 2) << " %\r";
        cout.flush();
    }
};