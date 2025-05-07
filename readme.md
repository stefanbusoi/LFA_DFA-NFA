# Regex to DFA

This project implements a converter from **Regular Expressions (Regex)** to **Deterministic Finite Automata (DFA)** using C++. 

## Project structure
### I have 3 classes
- DFA (used to model a determinitic finite automata)
- NFA (used to model a nedeterministic finite automata)
- NFA_LAMBDA(used to model a nedeterministic finite automata with lambda transition)
  
## How it works

### It has 2 main steps:
- reads the regular expresion and the testcases from [LFA-Assignment2_Regex_DFA.json](LFA-Assignment2_Regex_DFA.json) (check the file to see the format of the json, it needs to be in the same folder with the executoable or one level above)
- transform a regular expresion to a lambda-nfa
  - Transform the regular expresion in a postfix form using the Shunting-Yard algorithm
  - Build an lambda-nfa using Thompson algorithm
- transform a lambda-nfa in to a dfa using subset construction

## How to run it

### Clion (Recomended)
- Open the project in CLion.
- CLion will auto-detect CMakeLists.txt and configure the build.
- Press ctrl+f10 to build it and shift+f9 to run the program

### Linux(Terminal + Cmake)
```
mkdir -p build && cd build
cmake ..
make
./RegexToDFA
```
