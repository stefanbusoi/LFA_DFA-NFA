# Regex to DFA

This project implements a context free grammar using C++. 

## Project structure
### I have 1 classes
- GCD (this manages all things about a context free grammar)
  
## How it works
  The context free grammar consider only [A-Z] as being nonterminals, anything else is terminal
### How it generate strings:
  Takes random steps from the starting symbol until it gets to a string with no nonterminals
### How it check if a string is in a grammar
  to check if a word is in a GCD it takes random steps and see if it gets to the wanted string
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
