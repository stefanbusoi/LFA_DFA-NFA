
#include "DFA.h"

#include <fstream>
#include <sstream>

DFA::DFA(std::set<std::string> Q, std::set<std::string> s_symbols, std::map<std::string, std::map<std::string, std::string>> q, std::string q0, std::set<std::string> N):
    states(std::move(Q)),
    symbols(std::move(s_symbols)),
    transitions(std::move(q)),
    initial_State(std::move(q0)),
    final_States(std::move(N)) {

}

DFA DFA::readFromFile(const std::string &FilePath) {
    std::set<std::string> states;
    std::set<std::string> symbols;
    std::map<std::string,std::map<std::string,std::string>> transitions;
    std::string initial_State;
    std::set<std::string> final_States;
    std::ifstream file(FilePath);
    enum class InputType {
        Nothing = 0,
        Symbols=1,
        States=2,
        Transitions=3
    }InputType=InputType::Nothing;
    while (!file.eof()) {
        std::string line;
        std::getline(file, line);
        if (line[0]=='#')
            continue;
        if (line=="End") {
            InputType=InputType::Nothing;
            continue;
        }
        switch (InputType) {
            case InputType::Symbols: {
                symbols.insert(line);
                break;
            }
            case InputType::States: {
                std::string stateString;
                std::istringstream stream(line);
                stream>>stateString;
                if (stateString[stateString.size()-1]==',') {
                    stateString.erase(stateString.size()-1);
                    char specialState;
                    while (stream) {
                        stream>>specialState;
                        if (specialState=='S') {
                            if (initial_State!="")
                                throw std::invalid_argument("DFA::initial_State Too many initial States");
                            initial_State=stateString;
                        }else if (specialState=='F') {
                            final_States.insert(stateString);
                        }
                    }
                }
                states.insert(stateString);
                break;
            }
            case InputType::Transitions: {
                std::string StartState,Symbol,EndState;
                std::istringstream stream(line);
                stream>>StartState>>Symbol>>EndState;
                StartState.erase(StartState.size()-1);
                Symbol.erase(Symbol.size()-1);
                transitions[StartState][Symbol]=EndState;
            }
            case InputType::Nothing: {
                if (line=="Sigma:") {
                    InputType=InputType::Symbols;
                }
                if (line=="States:") {
                    InputType=InputType::States;
                }
                if (line=="Transitions:") {
                    InputType=InputType::Transitions;
                }
            }
        }
    }
    for (const auto&[state, secondPart]:transitions) {
        if (states.find(state)==states.end()) {
            throw std::invalid_argument("DFA::state\""+state+"\"not found");
        }
        for (const auto&[symbol, nextState]:secondPart) {
            if (symbols.find(symbol)==symbols.end()) {
                throw std::invalid_argument("DFA::symbol\""+symbol+"\" not found");
            }
            if (states.find(nextState)==states.end()) {
                throw std::invalid_argument("DFA::state\""+nextState+"\" not found");
            }

        }
    }
    return {states,symbols,transitions,initial_State,final_States};
}

std::string DFA::nextState(const std::string &CurrentState, const std::string &CurrentSymbol) {
    return transitions[CurrentState][CurrentSymbol];
}

bool DFA::testWord(const std::vector<std::string> &word) {
    std::string state=initial_State;
    for (const auto& letter:word) {
        state=nextState(state,letter);
    }
    if (final_States.contains(state))
        return true;
    return false;
}
