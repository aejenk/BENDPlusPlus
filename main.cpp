#include "bin/binbend.h"
#include "bin/hexbend.h"

using namespace std;

vector<string> strSplit(string s, char delimit){
    stringstream ss (s);
    string segment;
    vector<string> seglist;

    while(getline(ss, segment, delimit)){
        seglist.push_back(segment);
    }

    return seglist;
}

void bendfile(BinBender bx, string name) {
    bx.mutate(1000, muts::SCATTER, true); // safely mutates 1000 bytes via SCATTERing
    bx.saveFile(name);
}

int main() {
    srand(time(0));

    cout << "Enter filename: ";
    string name;
    cin >> name;

    cout << endl;

    string savename;
    savename.assign(name);

    BinBender bx;
    bx.loadFile(name);

    for(int i = 0; i < 10; i++){
        savename.insert(savename.find("."), to_string(i));
        bendfile(bx, savename);
        savename.assign(name);
    }

    return 0;
}