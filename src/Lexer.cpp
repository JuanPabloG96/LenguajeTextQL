#include "Lexer.hpp"
#include "Data/LexerData.hpp"
#include "Data/TransitionMatrix.hpp"
#include <cctype>
#include <iomanip>
#include <iostream>

Lexer::Lexer() : state(0), category(0), line(1), column(0) {}

void Lexer::createTokenList(char c) {
  auto symbol = classifyChar(c);
  category = static_cast<int>(symbol);
  state = lexer::transitionMatrix[state][category];

  if (state < 0) {
    auto error = lexer::LexicalErrorMessages.find(state);

    if (error != lexer::LexicalErrorMessages.end()) {
      errors.push_back({line, column, error->second, buffer});
    }
    state = 0;
    buffer.clear();
  } else if (state >= 1000) {
    auto token = lexer::TokenMap.find(state);

    if (token != lexer::TokenMap.end()) {
      auto lexeme = buffer;
      while (!lexeme.empty() &&
             (lexeme.back() == ' ' || lexeme.back() == '\n' || lexeme.back() == '\t')) {
        lexeme.pop_back();
      }
      TokenList.add(token->second, lexeme);
    }
    state = 0;
    buffer.clear();
  } else {
    if (symbol != lexer::Symbol::Delimitador)
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

void Lexer::printTokenList() {
  auto current = TokenList.getHead();

  std::cout << std::endl;
  std::cout << std::left << std::setw(20) << "#" << std::setw(50) << "Tipo"
            << "Token" << std::endl;

  std::cout << std::string(100, '-') << std::endl;

  while (current) {
    std::cout << std::left << std::setw(20) << current->getId() << std::setw(50)
              << current->getTokenType() << current->getToken() << std::endl;

    current = current->getNext();
  }
}
void Lexer::printErrors() {
  for (auto error : errors) {
    std::cout << "Error en la linea " << error.line << ", columna " << error.column << ": "
              << error.message << " (" << error.lexeme << ")." << std::endl;
  }
}
