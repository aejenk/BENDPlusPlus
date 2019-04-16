#include <string>
#include <chrono>
#include <iostream>
#include <iomanip>

using namespace std;
using namespace std::chrono;


// Used for easier use of the function.
#define takeTimeAndReturn(name, function) time_calculate_return(name, [&]{return function;})
#define takeTimeWithoutReturn(name, function) time_calculate_void(name, [&]{return function;})

// Calculates the time for a function that has a return type.
// l should always be a lambda CONTAINING a call to the function.
template<typename F>
auto time_calculate_return(string fname, F l) -> decltype(l()){
    auto start = high_resolution_clock::now();
    auto ret = l(); // executes function and retrieves return type
    auto end = high_resolution_clock::now();
    duration<float> diff = end-start;
    cout << "Time taken by ["  << fname << "] : " << diff.count() << "s";
    return ret; // returns the return value of the function
}

// Calculates the time for a function that has no return type.
// l should always be a lambda CONTAINING a call to the function.
template<typename F>
auto time_calculate_void(string fname, F l) -> void {
    auto start = high_resolution_clock::now();
    l(); // executes function
    auto end = high_resolution_clock::now();
    duration<float> diff = end-start;
    cout << "Time taken by ["  << fname << "] : " << diff.count() << "s";
}

/*
    Uses:
        - Use the macro in order to use the function for ease of use.
        - l is meant to be a lambda, hence why the macro is used.
        - ex: taketime("name", &str.erase(1,5));
        - ex: string s = taketime("lk", str.substr(1,5));
        - Whenever the return value is not assigned, the function needs to be dereferenced.
*/