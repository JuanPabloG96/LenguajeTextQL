#include <string>

#include "Lexer.hpp"
#include "Parser.hpp"

int main() {
  Lexer lexer;
  std::string filename = "../docs/test5.txt";

  lexer.analizeFile(filename);
  lexer.printErrors();

  Parser parser(lexer.getTokenList());
  parser.parse();

  return 0;
}
