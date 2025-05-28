
#include "NFA.h"

#include <fstream>
#include <sstream>

NFA::NFA(std::set<std::string> Q, std::set<std::string> s_symbols, std::map<std::string, std::map<std::string, std::set<std::string>>> q, std::string q0, std::set<std::string> N):
    states(std::move(Q)),
    symbols(std::move(s_symbols)),
    transitions(std::move(q)),
    initial_State(std::move(q0)),
    final_States(std::move(N)) {

}

NFA NFA::readFromFile(const std::string &FilePath) {
    std::set<std::string> states;
    std::set<std::string> symbols;
    std::map<std::string,std::map<std::string,std::set<std::string>>> transitions;
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
                transitions[StartState][Symbol].insert(EndState);
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
    for (const auto& [state, map2]:transitions) {
        if (!states.contains(state)) {
            throw std::invalid_argument("NFA::state\""+state+"\"not found");
        }
        for (const auto&[simb,nextStates]:map2) {
            if (!symbols.contains(simb)) {
                throw std::invalid_argument("NFA::symbol\""+simb+"\" not found");
            }
            for (const auto& nextState:nextStates) {
                if (!states.contains(nextState)) {
                    throw std::invalid_argument("NFA::state\""+nextState+"\" not found");
                }
            }
        }
    }
    return {states,symbols,transitions,initial_State,final_States};
}

std::set<std::string> NFA::nextState(const std::string &CurrentState, const std::string &CurrentSymbol) {
    return transitions[CurrentState][CurrentSymbol];
}

bool NFA::testWord(const std::vector<std::string> &word) {
    std::set<std::string> CurrentStates=(std::set<std::string>){initial_State};
    for (const auto& letter:word) {
        std::set<std::string> NextStates;
        for (const auto& CurrentState:CurrentStates) {
            for (const auto& state:nextState(CurrentState,letter))
                NextStates.insert(state);
        }
        
        CurrentStates=NextStates;
    }
    for (const auto& state:CurrentStates) {
        if (final_States.contains(state) )
            return true;
    }
    return false;

}
