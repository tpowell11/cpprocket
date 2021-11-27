/*
Simple active command line interface library
STD=C++
Tom Powell 2021
*/
#include <iostream>
#include <string>
namespace cui{
// Simple prompted input
    template<typename t>
    t promptInput(std::string prompt){
        t inp;
        std::cout << prompt;
        std::cin >> (t)inp;
        return inp;
    }
// Print from file
    void printFile(std::string filename) {
        std::ifstream f(filename);

    }
}