#pragma once
#include <map>
#include <set>
#include <string>
#include <vector>

class DFA{
    std::set<std::string> states;
    std::set<std::string> symbols;
    std::map<std::string,std::map<std::string,std::string>> transitions;
    std::string initial_State;
    std::set<std::string> final_States;
public:
    bool testWord(const std::vector<std::string> &word);
    DFA(std::set<std::string> Q,std::set<std::string>s_symbols,std::map<std::string,std::map<std::string,std::string>> q,std::string q0,std::set<std::string> N);
    explicit DFA(const std::string &FilePath) : DFA(readFromFile(FilePath)) {}
private:
    static DFA readFromFile(const std::string &FilePath);
    std::string nextState(const std::string &CurrentState, const std::string &CurrentSymbol);

};
