# BENDPlusPlus
A databender made in C++. A work in progress that might not be too functional...

## Progress
There are two objects, `BinBender` and `HexBender`.

`HexBender` works by converting the file into hexadecimal, and manipulating the hex code, then saving it back into a binary file. For now, it offers no benefits other than being able to manipulate the data in a more usable hex format.

`BinBender` works by loading the file and manipulating it directly. This results in a *bender* that is **12 times faster** than the `HexBender`. However, the data is far less easy to manipulate structurally, since it's all garbage characters rather than hexadecimal - however considering the purpose is to ***bend***, that is hardly an issue.

`BinBender` also saves/loads files using custom buffers. For files under 1GB, the buffer size is 1KB (Max of 1,000,000 buffer runs), and for over the buffer size increases to 1MB (>1,000 buffer runs). These buffer numbers may change/be more flexible in the future via configurations.

In the future, configurations for the `BinBender` may be made availiable using a `configure` function.

## Compilation
Compiling is as easy as running `make`. 
I suggest you install make if you do not have it - but if you'd rather directly use a terminal command:

`g++ -o m -g -m64 main.cpp bin/binbend.cpp bin/binbend.h bin/hexbend.cpp bin/hexbend.h`

**Please note**: You will need an x86-64 bit version of `g++` in order to have the full functionality of the program. Otherwise, you have a memory restriction of approximately 4GB. For files under than that you can use the i686/32-bit version. This is due to memory limitations with 32-bit systems.