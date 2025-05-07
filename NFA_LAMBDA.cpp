
#include "NFA_LAMBDA.hpp"

#include <fstream>
#include <iostream>
#include <queue>

#include "NFA.h"
NFA_LAMBDA::NFA_LAMBDA(std::set<std::string> Q, std::set<std::string> s_symbols, std::map<std::string, std::map<std::string, std::set<std::string>>> q, std::string q0, std::set<std::string> N):
    states(std::move(Q)),
    symbols(std::move(s_symbols)),
    transitions(std::move(q)),
    initial_State(std::move(q0)),
    final_States(std::move(N)) {

}

NFA_LAMBDA::NFA_LAMBDA(const std::string &regex) {
    std::string revPolishNot = PostFixRegex(regex);
    std::stack<NFA_LAMBDA> st;
    int index=0;
    std::cout<<revPolishNot<<std::endl;
    for (auto token:revPolishNot) {
        if (isSymbol(token)) {
            std::string state1=std::to_string(index++);
            std::string state2=std::to_string(index++);
            //STATES;
            std::set<std::string> states={state1,state2};

            //SYMBOLS
            std::set<std::string> symbols={std::string(1, token)};

            //TRANSITION FUNCTION
            std::map<std::string,std::map<std::string,std::set<std::string>>> transitions;
            transitions[state1][std::string(1, token)].insert(state2);

            //INITIAL STATE
            std::string initial_State=state1;

            //FINAL STATE
            std::set<std::string> final_States={state2};
            st.emplace(states,symbols,transitions,initial_State,final_States);
        }
        if (token=='.') {
            auto LNFA1=st.top();
            st.pop();
            auto& LNFA2=st.top();
            //STATES
            LNFA2.states.merge(LNFA1.states);
            //SYMBOLS
            LNFA2.symbols.merge(LNFA1.symbols);
            //TRANSITION FUNCTION
            LNFA2.transitions.merge(LNFA1.transitions);
            //INITIAL STATE
            for (const auto& final_state:LNFA2.final_States) {
                LNFA2.transitions[final_state][""].insert(LNFA1.initial_State);
            }
            //FINAL STATE
            LNFA2.final_States=LNFA1.final_States;
        }
        if (token=='?') {
            auto& LNFA=st.top();
            for (const auto& finalState:LNFA.final_States) {
                LNFA.transitions[LNFA.initial_State][""].insert(finalState);
            }
        }
        if (token=='+') {
            auto& LNFA=st.top();
            for (const auto& finalState:LNFA.final_States) {
                LNFA.transitions[finalState][""].insert(LNFA.initial_State);
            }
        }
        if (token=='*') {
            auto& LNFA=st.top();
            for (const auto& finalState:LNFA.final_States) {
                LNFA.transitions[finalState][""].insert(LNFA.initial_State);
                LNFA.transitions[LNFA.initial_State][""].insert(finalState);
            }
        }
        if (token=='|') {
            auto LNFA1=st.top();
            st.pop();
            auto& LNFA2=st.top();
            //STATES
            LNFA2.states.merge(LNFA1.states);
            //SYMBOLS
            LNFA2.symbols.merge(LNFA1.symbols);
            //TRANSITION FUNCTION
            LNFA2.transitions.merge(LNFA1.transitions);
            //INITIAL STATE
            std::string state1=std::to_string(index++);
            LNFA2.transitions[state1][""].insert(LNFA1.initial_State);
            LNFA2.transitions[state1][""].insert(LNFA2.initial_State);
            LNFA2.initial_State=state1;
            //FINAL STATE
            LNFA2.final_States.merge(LNFA1.final_States);
        }
    }
    *this=st.top();
}

std::set<std::string> NFA_LAMBDA::nextState(const std::string &CurrentState, const std::string &CurrentSymbol) const{
    try {
        return transitions.at(CurrentState).at(CurrentSymbol);
    }catch (std::out_of_range e) {
        return std::set<std::string>();
    }
}

int NFA_LAMBDA::priority(const char op) {
    if (op=='*' or op=='+' or op=='?') {
        return 3;
    }
    if (op=='.')
        return 2;
    if (op=='|')
        return 1;
    return 0;

}

bool NFA_LAMBDA::isSymbol(const char x) {
    if (x>='0' and x<='9')
        return true;
    if (x>='A' and x<='Z')
        return true;
    if (x>='a' and x<='z')
        return true;
    return false;
}

std::string NFA_LAMBDA::PostFixRegex(const std::string &regex) {
    std::stack<char> stack;
    std::string str;
    std::string ans;
    bool Symbol=false;
    for (char entry:regex) {
        if (Symbol and (entry=='(' or isSymbol(entry))) {
            str+='.';
        }
        if (entry==')' or isSymbol(entry) or entry=='*' or entry=='?' or entry=='+') {
            Symbol=true;
        }
        else {
            Symbol=false;
        }
        str+=entry;
    }
    for (char entry:str) {
        if (isSymbol(entry)) {
            ans.push_back(entry);
            continue;
        }
        if (entry==')') {
            while (!stack.empty() and stack.top()!='(') {
                ans.push_back(stack.top());
                stack.pop();
            }
            if (!stack.empty() and stack.top()=='(') {
                stack.pop();
            }
            continue;
        }
        if (entry=='(') {
            stack.push(entry);
            continue;
        }
        int pr=priority(entry);
        while (!stack.empty() and pr<priority(stack.top()) ) {
            ans.push_back(stack.top());
            stack.pop();
        }
        stack.push(entry);
    }
    while (!stack.empty()) {
        ans.push_back(stack.top());
        stack.pop();
    }
    return ans;

}

bool NFA_LAMBDA::testWord(const std::vector<std::string> &word) {

    std::set<std::string> CurrentStates=(std::set<std::string>){initial_State};
    std::queue<std::string> queue;
    queue.push(initial_State);
    while (!queue.empty()) {
        std::string currentState=queue.front();
        queue.pop();
        for (const auto& state:nextState(currentState,"")){
            if (!CurrentStates.contains(state)) {
                queue.push(state);
            }
            CurrentStates.insert(state);
        }
    }
    for (const auto& letter:word) {
        std::set<std::string> NextStates;
        for (const auto& CurrentState:CurrentStates) {
            for (const auto& state:nextState(CurrentState,letter))
                NextStates.insert(state);
        }
        for (const auto& i:NextStates) {
            queue.push(i);
        }
        while (!queue.empty()) {
            std::string currentState=queue.front();
            queue.pop();
            for (const auto& state:nextState(currentState,"")){
                if (!NextStates.contains(state)) {
                    queue.push(state);
                }
                NextStates.insert(state);
            }
        }

        CurrentStates=NextStates;
    }
    for (const auto& state:CurrentStates) {
        if (final_States.contains(state) )
            return true;
    }
    return false;

}
bool NFA_LAMBDA::debugWord(const std::vector<std::string> & word) {
    std::set<std::string> CurrentStates=(std::set<std::string>){initial_State};
    //First lambda transitions
    std::queue<std::string> queue;
    queue.push(initial_State);
    while (!queue.empty()) {
        std::string currentState=queue.front();
        queue.pop();
        for (const auto& state:nextState(currentState,"")){
            if (!CurrentStates.contains(state)) {
                queue.push(state);
            }
            CurrentStates.insert(state);
        }
    }
    for (const auto& letter:word) {
        std::set<std::string> NextStates;
        for (const auto& CurrentState:CurrentStates) {
            for (const auto& state:nextState(CurrentState,letter))
                NextStates.insert(state);
        }
        std::queue<std::string> queue;
        for (const auto& i:NextStates) {
            queue.push(i);
        }
        while (!queue.empty()) {
            std::string currentState=queue.front();
            queue.pop();
            for (const auto& state:nextState(currentState,"")){
                if (!NextStates.contains(state)) {
                    queue.push(state);
                }
                NextStates.insert(state);
            }
        }
        for (auto i:NextStates) {
            std::cout<<i<<" ";
        }
        std::cout<<"\n";
        CurrentStates=NextStates;
    }
    for (const auto& state:CurrentStates) {
        if (final_States.contains(state) )
            return true;
    }
    return false;

}

DFA NFA_LAMBDA::to_DFA() const{
    std::set<std::string> m_states;
    std::set<std::string> m_symbols;
    std::map<std::string,std::map<std::string,std::string>> m_transitions;
    std::string m_initial_State;
    std::set<std::string> m_final_States;


    std::set<std::string> CurrentStates=(std::set<std::string>){initial_State};
    std::queue<std::string> queue;
    int index=0;
    std::map<std::set<std::string>,std::string> mp;
    queue.push(initial_State);
    while (!queue.empty()) {
        std::string currentState=queue.front();
        queue.pop();
        for (const auto& state:nextState(currentState,"")){
            if (!CurrentStates.contains(state)) {
                queue.push(state);
            }
            CurrentStates.insert(state);
        }
    }
    mp[CurrentStates]= std::to_string( index++);
    m_states.insert(mp[CurrentStates]);
    m_initial_State=mp[CurrentStates];
    std::queue<std::set<std::string>> state;
    state.push(CurrentStates);
    while (!state.empty()) {
        CurrentStates=state.front();
        state.pop();
        for (auto symbol:symbols) {
            std::set<std::string> NextStates;
            for (const auto& CurrentState:CurrentStates) {
                for (const auto& state:nextState(CurrentState,symbol))
                    NextStates.insert(state);
            }
            for (const auto& i:NextStates) {
                queue.push(i);
            }
            while (!queue.empty()) {
                std::string currentState=queue.front();
                queue.pop();
                for (const auto& state:nextState(currentState,"")){
                    if (!NextStates.contains(state)) {
                        queue.push(state);
                    }
                    NextStates.insert(state);
                }
            }
            if (mp.find(NextStates) == mp.end()) {
                mp[NextStates]=std::to_string(index++);
                m_states.insert(mp[NextStates]);
                state.push(NextStates);
            }
            m_transitions[mp[CurrentStates]][symbol]=mp[NextStates];
            for (auto final_state:final_States) {
                if (NextStates.find(final_state)!=NextStates.end()) {
                    m_final_States.insert(mp[NextStates]);
                }
            }
        }
    }
    m_symbols=this->symbols;


    return DFA(m_states,m_symbols,m_transitions,m_initial_State,m_final_States);
}
