//
// Created by stefa on 5/22/2025.
//

#include "CFG.hpp"


bool CFG::isTerminal(char c) {
  if (c>='A' and c<='Z') {
    return false;
  }
  if (c>='a' and c<='z') {
    return true;
  }
  return true;
}

int CFG::numberOfTerminals(std::string s) {
  int nr=0;
  for (auto i:s) {
      if (isTerminal(i)) {
        nr++;
      }
  }
  return nr;
}

CFG::CFG(const std::vector<production> &productions, char startSymbol) {
  this->startSymbol = startSymbol;
  for (const auto& [nonTerm,Result]:productions) {
    this->productions[nonTerm].push_back(Result);
  }
  std::queue<char> q;
  std::set<char> elements;
  q.push(startSymbol);
  while (!q.empty()) {
    char PossibleTerminal = q.front();
    q.pop();
    elements.insert(PossibleTerminal);

    if (isTerminal(PossibleTerminal)) {
      terminals.insert(PossibleTerminal);
      continue;
    }
    nonTerminals.insert(PossibleTerminal);
    for (auto &[nonTerminal,generatedString]: productions) {
      if (nonTerminal != PossibleTerminal) {
        continue;
      }
      for (auto element: generatedString) {
        if (!elements.contains(element)) {
          q.push(element);
          elements.insert(element);
        }
      }
    }
  }
}

bool CFG::testMembership(const std::string &word, int max_depth) {
  std::unordered_set<std::string> Visited;
  std::unordered_map<std::string,int> ToVisit;
  ToVisit[std::string(1, startSymbol)]=max_depth;
  while (!ToVisit.empty()) {
    std::string currentProduction = ToVisit.begin()->first;
    int depth=ToVisit[currentProduction];

    ToVisit.erase(currentProduction);
    Visited.insert(currentProduction);
    if (depth==0) {
      continue;
    }
    if (currentProduction == word) {
      return true;
    }
    if (numberOfTerminals(currentProduction) > word.size()) {
      continue;
    }
    for (size_t i = 0; i < currentProduction.size(); i++) {
      if (isTerminal(currentProduction[i])) {
        if (currentProduction[i] != word[i]) {
          break;
        }
        continue;
      }
      for (auto &generatedString: productions[currentProduction[i]]) {
        if (currentProduction.size() > word.size()&&!generatedString.empty()) {
          continue;
        }
        std::string newProduction = currentProduction;
        newProduction.replace(i,1, generatedString);
        if (!Visited.contains(newProduction)) {
          ToVisit[newProduction]=depth-1;
        }
      }
    }
    continue;
  }
  return false;
}

std::vector<std::string> CFG::getElements(const int nrElements, const int maxLenght) {
  std::vector<std::string> result;
  std::unordered_set<std::string> Visited;
  std::vector<std::string> ToVisitVect;
  std::set<std::string> ToVisit;
  ToVisit.insert(std::string(1, startSymbol));
  ToVisitVect.push_back(std::string(1, startSymbol));
  while (!ToVisit.empty() && result.size() < nrElements) {

    RandomIntGenerator RandInt{0, static_cast<int>(ToVisitVect.size()) - 1};
    int index=RandInt.generate();
    std::string currentProduction = ToVisitVect[index];

    ToVisit.erase(currentProduction);
    swap(ToVisitVect[index],ToVisitVect.back());
    ToVisitVect.pop_back();

    Visited.insert(currentProduction);

    bool HasNonTerminal = false;

    for (size_t i = 0; i < currentProduction.size(); i++) {
      if (isTerminal(currentProduction[i])) {
        continue;
      }
      HasNonTerminal = true;
      for (const auto& generatedString: productions[currentProduction[i]]) {
        if (maxLenght < currentProduction.size()) {
          continue;
        }

        std::string newProduction = currentProduction;
        newProduction.replace(i,1, generatedString);

        if (!Visited.contains(newProduction)) {
          if (!ToVisit.contains(newProduction)) {
            ToVisit.insert(newProduction);
            ToVisitVect.push_back(newProduction);
          }
        }
      }
      break;
    }
    if (!HasNonTerminal) {
      result.push_back(currentProduction);
    }
  }
  return result;
}

std::vector<UsedProd> CFG::getProductions(const std::string &word, const int max_depth) {
  if (!testMembership(word,max_depth)) {
    return {};
  }
  std::unordered_set<std::string> Visited;
  std::unordered_map<std::string,std::vector<UsedProd>> ToVisit;
  ToVisit[std::string(1, startSymbol)]={};
  while (!ToVisit.empty()) {
    std::string currentProduction = ToVisit.begin()->first;
    auto usedProd=ToVisit[currentProduction];
    int depth=ToVisit[currentProduction].size();
    if (currentProduction == word) {
      return ToVisit[currentProduction];
    }
    ToVisit.erase(currentProduction);
    Visited.insert(currentProduction);
    if (depth>max_depth) {
      continue;
    }
    if (numberOfTerminals(currentProduction) > word.size()) {
      continue;
    }
    for (size_t i = 0; i < currentProduction.size(); i++) {
      if (isTerminal(currentProduction[i])) {
        if (currentProduction[i] != word[i]) {
          break;
        }
        continue;
      }
      for (auto &generatedString: productions[currentProduction[i]]) {
        if (currentProduction.size() > word.size()&&!generatedString.empty()) {
          continue;
        }
        std::string newProduction = currentProduction;
        newProduction.replace(i,1, generatedString);
        if (!Visited.contains(newProduction)) {
          auto currentVect=usedProd;
          currentVect.push_back(UsedProd{currentProduction[i],currentProduction,generatedString,newProduction});
          ToVisit[newProduction]=currentVect;
        }
      }
      break;
    }
  }
  return {};
}
