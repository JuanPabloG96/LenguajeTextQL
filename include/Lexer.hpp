#ifndef LEXER_HPP
#define LEXER_HPP

#include "Data/TransitionMatrix.hpp"

class Lexer {
private:
  int state;
  int category;

public:
  Lexer();
  lexer::Symbol clasifyChar(char c);
};

#endif
