#pragma once
#include <map>
#include <set>
#include <string>
#include <vector>

class NFA{
    std::set<std::string> states;
    std::set<std::string> symbols;
    std::map<std::string,std::map<std::string,std::set<std::string>>> transitions;
    std::string initial_State;
    std::set<std::string> final_States;
public:
    bool testWord(const std::vector<std::string> &word);
    NFA(std::set<std::string> Q,std::set<std::string> s_symbols,std::map<std::string,std::map<std::string,std::set<std::string>>> q,std::string q0,std::set<std::string> N);;
    explicit NFA(const std::string &FilePath) : NFA(readFromFile(FilePath)) {}
private:
    static NFA readFromFile(const std::string &FilePath);
    std::set<std::string> nextState(const std::string &CurrentState, const std::string &CurrentSymbol);
};
