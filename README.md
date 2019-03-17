# BENDPlusPlus
A databender made in C++. A work in progress that might not be too functional...

## Progress
Two methods of databending are possible, HEX and ASCII. 
ASCII has been found to be ~12x faster than the HEX method, however HEX is being kept as a legacy, or incase of future convenience.

There is one limitation that the code cannot modify files that are too large. This is going to be fixed in a later commit however using memory mapping.