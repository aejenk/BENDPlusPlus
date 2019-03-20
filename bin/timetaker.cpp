#include <string>
#include <chrono>
#include <iostream>

using namespace std;
using namespace std::chrono;

#define taketime(name, function) time_calculate(name, [&]{return function;}); 

template<typename F>
auto time_calculate(string fname, F l) -> decltype(l()){
    auto start = high_resolution_clock::now(); 
    auto ret = l();
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start); 
    cout << "Time taken by ["  << fname << "] : " << duration.count() << endl;
    return ret;
}

/*
    Uses:
        - Use the macro in order to use the function for ease of use.
        - l is meant to be a lambda, hence why the macro is used.
        - ex: taketime("name", &str.erase(1,5));
        - ex: string s = taketime("lk", str.substr(1,5));
        - Whenever the return value is not assigned, the function needs to be dereferenced.
*/