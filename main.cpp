//Busoi-Radulescu Stefan
//Grupa 152
//C++20

#include <fstream>
#include <iostream>
#include <vector>

#include "DFA.h"
#include "NFA.h"


int main() {

    DFA AFD("TestAFD.txt");
    std::cout<<AFD.testWord(std::vector<std::string>({"b","b","a","a","b","b"}));

    NFA AFN("TestAFN.txt");
    std::cout<<AFN.testWord(std::vector<std::string>({"b","a","a","a","b","a"}));
    return 0;
}
