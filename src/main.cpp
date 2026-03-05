#include "Lexer.hpp"
#include <fstream>
#include <iostream>

int main() {

  std::ifstream archivo("../docs/input.txt");
  if (!archivo.is_open()) {
    std::cerr << "Error: no se pudo abrir el archivo\n";
    return -1;
  }

  char c;
  Lexer lexer;

  while (archivo.get(c)) {
    lexer.createTokenList(c);
  }

  lexer.printTokenList();

  archivo.close();
  return 0;
}
