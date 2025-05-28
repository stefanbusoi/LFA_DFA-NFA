//Busoi-Radulescu Stefan
//Grupa 152
//C++20
#include <format>
#include <fstream>
#include <iostream>
#include <stack>
#include <vector>
#include <nlohmann/json.hpp>

#include "CFG.hpp"
#include "DFA.h"
#include "NFA.h"
#include "NFA_LAMBDA.hpp"
using json=nlohmann::json;

int main() {
  CFG x({{'S',"SS"},
    {'S',"(S)"},
  {'S',""}},'S');

  std::vector<std::string>ans = x.getElements(10 ,50);
  for (const auto& word:ans) {
    std::cout<<word<<std::endl;
 }
  std::cout<<(x.testMembership("((()))()(((())))", 30)?"DA":"NU")<<"\n";
  std::vector<UsedProd> prods=x.getProductions("((()))()(((())))",30);
  for (auto i:prods) {
    std::cout<<i.from<<" => "<<i.to<<"|"<<" used derivate "<<i.nonterminal<<" to "<<i.prod<<std::endl;
  }
  return 0;
}
