#pragma once
#include <map>
#include <set>
#include <stack>
#include <string>
#include <vector>

#include "DFA.h"

class NFA_LAMBDA{
  std::set<std::string> states;
  std::set<std::string> symbols;
  std::map<std::string,std::map<std::string,std::set<std::string>>> transitions;
  std::string initial_State;
  std::set<std::string> final_States;

  static int priority(char op);

  static bool isSymbol(char x);

  static std::string PostFixRegex(const std::string & regex);

public:
  bool testWord(const std::vector<std::string> &word);

  bool debugWord(const std::vector<std::string> & vector);

  DFA to_DFA() const;

  NFA_LAMBDA(std::set<std::string> Q,std::set<std::string> s_symbols,std::map<std::string,std::map<std::string,std::set<std::string>>> q,std::string q0,std::set<std::string> N);;
  NFA_LAMBDA(const std::string &regex);

private:
  std::set<std::string> nextState(const std::string &CurrentState, const std::string &CurrentSymbol) const;
};

