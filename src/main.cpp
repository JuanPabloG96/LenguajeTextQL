#include <string>

#include "Lexer.hpp"
#include "Parser.hpp"

int main() {
  Lexer lexer;
  std::string filename = "../docs/test2.txt";

  lexer.analizeFile(filename);

  Parser parser(lexer.getTokenList());
  parser.parse();

  return 0;
}
