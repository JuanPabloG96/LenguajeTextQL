#include "Parser.hpp"

#include <iostream>

Parser::Parser(const List& tokenList)
    : head(tokenList), current(tokenList.getHead()) {
  // clang-format off
    declaracion_map = {
        {"esquema", [this]() { defEsquema(); }},
        {"funcion",  [this]() { defFuncion(); }},
        {"fuente",   [this]() { asignacionFuente(); }},
    };

    sentencia_map = {
        {"si",       [this]() { condicional(); }},
        {"escanear", [this]() { escanear(); }},
        {"mostrar",  [this]() { mostrar(); }},
        {"exportar", [this]() { exportar(); }},
        {"retornar", [this]() { retorno(); }},
    };
  // clang-format on
}

bool Parser::match(const std::string& type) {
  return current->getTokenType() == type;
}

void Parser::advance() {
  if (current != nullptr && !match("EOF"))
    current = current->getNext();
}

void Parser::statements() {
  if (declaracion_map.contains(current->getToken())) {
    declaracion_map[current->getToken()]();
  } else if (sentencia_map.contains(current->getToken())) {
    sentencia_map[current->getToken()]();
  } else if (match("identificador")) {
    asignacion();
  } else {
    std::cerr << "Error: token inesperado: '" << current->getToken() << "'\n";
  }
}

// Parser.cpp
void Parser::defEsquema() {
}
void Parser::defFuncion() {
}
void Parser::asignacionFuente() {
}
void Parser::condicional() {
}
void Parser::escanear() {
}
void Parser::mostrar() {
}
void Parser::exportar() {
}
void Parser::retorno() {
}
void Parser::asignacion() {
}
