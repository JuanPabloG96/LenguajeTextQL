#ifndef LEXER_HPP
#define LEXER_HPP

#include "Data/TransitionMatrix.hpp"
#include "List.hpp"
#include <string>

class Lexer {
private:
  int state;
  int category;
  std::string buffer;
  List TokenList;

public:
  Lexer();
  lexer::Symbol classifyChar(char c);
  void createTokenList(char c);
  void printTokenList();
};

#endif
