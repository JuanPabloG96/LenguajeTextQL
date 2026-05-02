#include <string>

#include "Lexer.hpp"

int main() {
  Lexer lexer;
  std::string filename = "../docs/test1.txt";

  lexer.analizeFile(filename);
  lexer.printTokenList();
  lexer.printErrors();

  return 0;
}
