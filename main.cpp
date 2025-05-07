//Busoi-Radulescu Stefan
//Grupa 152
//C++20
#include <format>
#include <fstream>
#include <iostream>
#include <stack>
#include <vector>
#include <nlohmann/json.hpp>

#include "DFA.h"
#include "NFA.h"
#include "NFA_LAMBDA.hpp"
using json=nlohmann::json;

int main() {
    std::ifstream file;
    try {
        file=std::ifstream("../LFA-Assignment2_Regex_DFA.json");
    }catch(...) {
        file=std::ifstream("LFA-Assignment2_Regex_DFA.json");
    }
        json data=json::parse(file);
    for (auto entry:data) {
        std::string name=entry["name"].get<std::string>();
        std::string regex=entry["regex"].get<std::string>();
        std::cout<<std::format("Testing :{}: {}\n",name,regex);
        NFA_LAMBDA automata(regex);
        DFA DFAautomata=automata.to_DFA();
        for (auto test:entry["test_strings"]) {
            std::string input=test["input"].get<std::string>();
            std::vector<std::string> FormatedInput;
            for (auto i:input) {
                FormatedInput.push_back(std::string(1, i));
            }
            bool expectedResult=test["expected"].get<bool>();
            bool result=DFAautomata.testWord(FormatedInput);
            std::cout<<std::format("Input:{}, Expected Result:{}, Result:{}\n",input,expectedResult?"true ":"false",result?"true ":"false");
            if (result!=expectedResult) {
                std::cout<<std::flush;
                std::cout<<"Wrong Answer---------------------------------------"<<std::endl;
            }
        }
        std::cout<< std::endl;
    }

    std::cout<<"Press enter to exit the program";
    std::cin.get();
    return 0;
}
