#include <string>

#include "Lexer.hpp"
#include "Parser.hpp"

int main() {
  Lexer lexer;
  std::string filename = "../docs/test3.txt";

  lexer.analizeFile(filename);
  lexer.printErrors();
  lexer.printTokenList();

  Parser parser(lexer.getTokenList());
  parser.parse();

  return 0;
}
