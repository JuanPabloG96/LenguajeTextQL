#include "Lexer.hpp"
#include "Data/LexerData.hpp"
#include "Data/TransitionMatrix.hpp"
#include <cctype>
#include <iostream>

Lexer::Lexer() : state(0), category(0) {}

void Lexer::createTokenList(char c) {
  auto symbol = classifyChar(c);
  category = static_cast<int>(symbol);
  state = lexer::transitionMatrix[state][category];

  if (state < 0) {
    auto error = lexer::LexicalErrorMessages.find(state);

    if (error != lexer::LexicalErrorMessages.end()) {
      std::cout << error->second << std::endl;
    } else {
      std::cout << "Error desconocido";
    }
  } else if (state >= 1000) {
    auto token = lexer::TokenMap.find(state);

    if (token != lexer::TokenMap.end())
      TokenList.add(token->second, buffer);
    state = 0;
    buffer.clear();
  } else {
    buffer += c;
  }
}

lexer::Symbol Lexer::classifyChar(char c) {

  switch (c) {
  case 'a':
    return lexer::Symbol::A;
    break;
  case 'b':
    return lexer::Symbol::B;
    break;
  case 'c':
    return lexer::Symbol::C;
    break;
  case 'd':
    return lexer::Symbol::D;
    break;
  case 'e':
    return lexer::Symbol::E;
    break;
  case 'f':
    return lexer::Symbol::F;
    break;
  case 'h':
    return lexer::Symbol::H;
    break;
  case 'i':
    return lexer::Symbol::I;
    break;
  case 'l':
    return lexer::Symbol::L;
    break;
  case 'm':
    return lexer::Symbol::M;
    break;
  case 'n':
    return lexer::Symbol::N;
    break;
  case 'o':
    return lexer::Symbol::O;
    break;
  case 'p':
    return lexer::Symbol::P;
    break;
  case 'r':
    return lexer::Symbol::R;
    break;
  case 's':
    return lexer::Symbol::S;
    break;
  case 't':
    return lexer::Symbol::T;
    break;
  case 'u':
    return lexer::Symbol::U;
    break;
  case 'y':
    return lexer::Symbol::Y;
    break;
  case 'E':
    return lexer::Symbol::Exponente;
    break;
  case '.':
    return lexer::Symbol::Punto;
    break;
  case '_':
    return lexer::Symbol::GuionBajo;
    break;
  case '-':
    return lexer::Symbol::Menos;
    break;
  case '"':
    return lexer::Symbol::ComillasDobles;
    break;
  case '\'':
    return lexer::Symbol::ComillasSimples;
    break;
  }
  if (c == ' ' || c == '\n' || c == '\t')
    return lexer::Symbol::Delimitador;

  if (std::isdigit((unsigned char)c)) {
    if (c == '0')
      return lexer::Symbol::Cero;
    return lexer::Symbol::Digito;
  }

  if (std::isalpha((unsigned char)c))
    return lexer::Symbol::Letra;

  return lexer::Symbol::Error;
}

void Lexer::printTokenList() { TokenList.show(); }
