# BENDPlusPlus
A databender made in C++. A work in progress that might not be too functional...

## Progress
There are two objects, `BinBender` and `HexBender`.

`HexBender` works by converting the file into hexadecimal, and manipulating the hex code, then saving it back into a binary file. For now, it offers no benefits other than being able to manipulate the data in a more usable hex format.

`BinBender` works by loading the file and manipulating it directly. This results in a *bender* that is **12 times faster** than the `HexBender`. However, the data is far less easy to manipulate structurally, since it's all garbage characters rather than hexadecimal - however considering the purpose is to ***bend***, that is hardly an issue.

## Compilation
Compiling is as easy as running `make`. 
I suggest you install make if you do not have it - but if you'd rather directly use a terminal command:

`g++ -o m.exe main.cpp bin/binbender.cpp bin/hexbender.cpp`