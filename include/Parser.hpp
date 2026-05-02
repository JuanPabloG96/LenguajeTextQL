#pragma once

#include <functional>
#include <unordered_map>

#include "List.hpp"

class Parser {
 private:
  const List& head;
  Node* current;

  std::unordered_map<std::string, std::function<void()>> declaracion_map;
  std::unordered_map<std::string, std::function<void()>> sentencia_map;

  void statements();
  // inicio de statements
  void declaracion();
  void sentencia();
  // funciones de declaracion
  void defEsquema();
  void defFuncion();
  void asignacionFuente();
  // funciones de sentencia
  void condicional();
  void escanear();
  void mostrar();
  void exportar();
  void retorno();

  void asignacion();

  // funciones de control
  void advance();
  bool match(const std::string& type);

 public:
  Parser(const List& tokenList);
  void parse();
};
