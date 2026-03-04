#include "Data.hpp"
#include "MatrizTransicion.hpp"
#include <fstream>
#include <iostream>

Simbolo clasificar(char c) {
  if (std::isspace(c))
    return Simbolo::Delimitador;

  if (std::isdigit(c)) {
    if (c == '0')
      return Simbolo::Cero;
    return Simbolo::Digito;
  }

  if (std::isalpha(c))
    return Simbolo::Letra;

  switch (c) {
  case '.':
    return Simbolo::Punto;
  case '_':
    return Simbolo::GuionBajo;
  case '-':
    return Simbolo::Menos;
  case '"':
    return Simbolo::ComillasDobles;
  case '\'':
    return Simbolo::ComillasSimples;
  }
  return Simbolo::Error;
}

int main() {

  std::ifstream archivo("../docs/input.txt");
  if (!archivo.is_open()) {
    std::cerr << "Error: no se pudo abrir el archivo\n";
    return -1;
  }

  char c;

  archivo.close();
  return 0;
}
