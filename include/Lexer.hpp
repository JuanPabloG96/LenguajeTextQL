#pragma once

#include "Data/TransitionMatrix.hpp"
#include "List.hpp"
#include <string>
#include <vector>

struct LexicalErrors
{
    int line;
    int column;
    std::string message;
    std::string lexeme;
};

class Lexer
{
  private:
    int state;
    int category;
    int line;
    int column;
    std::string buffer;
    List TokenList;
    std::vector<LexicalErrors> errors;

  public:
    Lexer();
    Symbol classifyChar(char c);
    void createTokenList(char c);
    void lexerFinish();
    void printTokenList();
    void printErrors();
};
