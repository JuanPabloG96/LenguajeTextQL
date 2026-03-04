#ifndef LEXER_HPP
#define LEXER_HPP

class Lexer {
private:
  int state;
  int category;

public:
  Lexer();
  void classifyChar(char c);
};

#endif
