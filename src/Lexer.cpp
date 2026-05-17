#include "Lexer.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#include "Data/LexerData.hpp"
#include "Data/TransitionMatrix.hpp"

Lexer::Lexer() : state(0), category(0), line(1), column(-1) {
}

void Lexer::analizeFile(std::string& filename) {
  std::ifstream archivo(filename);
  if (!archivo.is_open()) {
    std::cerr << "Error: no se pudo abrir el archivo\n";
    return;
  }

  char c;
  Lexer lexer;

  while (archivo.get(c)) {
    if (c == '\n') {
      line++;
      column = -1;
    } else
      column++;
    createTokenList(c);
  }
  archivo.close();
  lexerFinish();
}

void Lexer::createTokenList(char c) {
  auto symbol = classifyChar(c);

  if (state == 0 && symbol == Symbol::Delimitador)
    return;

  category = static_cast<int>(symbol);
  state = transitionMatrix[state][category];

  // Si el estado es un error
  if (state < 0) {
    auto error = LexicalErrorMessages.find(state);

    if (c != '\n' && c != '\t') {
      buffer += c;
    }

    if (error != LexicalErrorMessages.end()) {
      errors.push_back({line, column, error->second, buffer});
    }

    state = 0;
    buffer.clear();
  }
  // Si el estado es de aceptación
  else if (state >= 1000) {
    auto lexeme = buffer;
    if (state == 3000 && PalabrasReservadas.count(lexeme)) {
      auto token = PalabrasReservadas.find(buffer);
      TokenList.add(*token, lexeme, line, column);
    } else {
      auto token = TokenMap.find(state);
      if (token != TokenMap.end()) {
        TokenList.add(token->second, lexeme, line, column);
      }
    }

    state = 0;
    buffer.clear();
    // Repetir el proceso para el siguiente caracter para evitar perder el token
    // de simbolos
    createTokenList(c);
  } else {
    buffer += c;
  }
}

void Lexer::lexerFinish() {
  if (state == 12) {
    auto error = "Error: no se a cerrado la cadena al final del archivo";
    buffer = buffer.substr(0, buffer.size() - 1);
    errors.push_back({line, column, error, buffer});
  }
  TokenList.add("EOF", "\0", line, column);
}

Symbol Lexer::classifyChar(char c) {
  switch (c) {
    case '.':
      return Symbol::Punto;
      break;
    case '_':
      return Symbol::GuionBajo;
      break;
    case '+':
      return Symbol::Mas;
      break;
    case '-':
      return Symbol::Menos;
      break;
    case '"':
      return Symbol::ComillaDoble;
      break;
    case '\'':
      return Symbol::ComillaSimple;
      break;
    case '>':
      return Symbol::Mayor;
      break;
    case ':':
      return Symbol::DosPuntos;
      break;
    case '~':
      return Symbol::Virgulilla;
      break;
    case '=':
      return Symbol::Igual;
      break;
    case '$':
      return Symbol::Dolar;
      break;
    case '@':
      return Symbol::Arroba;
      break;
    case '#':
      return Symbol::Hash;
      break;
    case '?':
      return Symbol::Interrogacion;
      break;
    case '&':
      return Symbol::Ampersand;
      break;
    case '|':
      return Symbol::BarraVertical;
      break;
    case '(':
      return Symbol::AbreParentesis;
      break;
    case ')':
      return Symbol::CierraParentesis;
      break;
    case '[':
      return Symbol::AbreCorchete;
      break;
    case ']':
      return Symbol::CierraCorchete;
      break;
    case '{':
      return Symbol::AbreLlave;
      break;
    case '}':
      return Symbol::CierraLlave;
      break;
    case ',':
      return Symbol::Coma;
      break;
  }

  if (c == ' ' || c == '\n' || c == '\t')
    return Symbol::Delimitador;

  if (std::isdigit((unsigned char)c)) {
    if (c == '0')
      return Symbol::Cero;
    return Symbol::Digito;
  }

  if (std::isalpha((unsigned char)c))
    return Symbol::Letra;

  return Symbol::Indefinido;
}

void Lexer::printTokenList() {
  auto current = TokenList.getHead();

  std::cout << std::endl;
  std::cout << std::left << std::setw(5) << "\n#" << std::setw(50) << "Tipo"
            << "Token" << std::endl;

  std::cout << std::string(80, '-') << std::endl;

  while (current) {
    std::cout << std::left << std::setw(5) << current->getId() << std::setw(50)
              << current->getTokenType() << current->getToken() << std::endl;

    current = current->getNext();
  }
  std::cout << "\n";
}
void Lexer::printErrors() {
  for (auto error : errors) {
    std::cout << "Error en la linea " << error.line << ", columna "
              << error.column << ": " << error.message << " (" << error.lexeme
              << ")." << std::endl;
  }
  std::cout << "\n";
}

const List& Lexer::getTokenList() const {
  return this->TokenList;
}
