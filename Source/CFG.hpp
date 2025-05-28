//
// Created by stefa on 5/22/2025.
//
#pragma once
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <ranges>
#include <set>
#include <string>
#include <vector>

#include "RandomNumberGenerator.hpp"
struct production {
  char nonterminal;
  std::string result;
};
struct UsedProd{
  char nonterminal;
  std::string from;
  std::string prod;
  std::string to;

};
class CFG {

  std::set<char> nonTerminals;
  std::set<char> terminals;
  std::map<char,std::vector<std::string>> productions;
  char startSymbol;
  RandomIntGenerator RandInt{0, 1};

  static bool isTerminal(char c);

  static int numberOfTerminals(std::string s);

public:
  CFG(const std::vector<production>& productions, char startSymbol);

  bool testMembership(const std::string &word, int max_depth);

  std::vector<std::string> getElements(const int nrElements, const int maxLenght = 10);

  template<typename T>
  T getRandomValue(const std::set<T> &production);
  template<typename T>
  T getRandomValue(const std::vector<T> &production) ;

  template<typename T,typename T2>
  std::pair<T,T2> getRandomValue(const std::map<T,T2> &production);

  std::vector<UsedProd> getProductions(const std::string& str, int i);
};

template<typename T>
T CFG::getRandomValue(const std::set<T> &production) {
  int x = production.size();
  RandomIntGenerator RandInt{0, x - 1};

  auto i = production.begin();
  std::advance(i, RandInt.generate());
  return *i;
}
template<typename T>
T CFG::getRandomValue(const std::vector<T> &production) {
  int x = production.size();
  RandomIntGenerator RandInt{0, x - 1};

  auto i = production.begin();
  return production.at(i);
}
template<typename T,typename T2>
std::pair<T,T2> CFG::getRandomValue(const std::map<T,T2> &production) {
  int x = production.size();
  RandomIntGenerator RandInt{0, x - 1};

  auto i = production.begin();
  std::advance(i, RandInt.generate());
  return *i;
}

