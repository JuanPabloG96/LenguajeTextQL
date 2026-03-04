#include "Lexer.hpp"
#include "Data/TransitionMatrix.hpp"
#include <cctype>

Lexer::Lexer() : state(0), category(0) {}

void Lexer::classifyChar(char c) {
  lexer::Symbol s;
  if (std::isspace(c))
    s = lexer::Symbol::Delimitador;

  if (std::isdigit(c)) {
    if (c == '0')
      s = lexer::Symbol::Cero;
    s = lexer::Symbol::Digito;
  }

  if (std::isalpha(c))
    s = lexer::Symbol::Letra;

  switch (c) {
  case '.':
    s = lexer::Symbol::Punto;
    break;
  case '_':
    s = lexer::Symbol::GuionBajo;
    break;
  case '-':
    s = lexer::Symbol::Menos;
    break;
  case '"':
    s = lexer::Symbol::ComillasDobles;
  case '\'':
    s = lexer::Symbol::ComillasSimples;
  }
  state = static_cast<int>(s);
}
