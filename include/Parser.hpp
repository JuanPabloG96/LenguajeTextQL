#pragma once

#include <functional>
#include <set>
#include <unordered_map>

#include "List.hpp"

class Parser {
private:
  const List &head;
  Node *current;

  std::unordered_map<std::string, std::function<void()>> declaracion_map;
  std::unordered_map<std::string, std::function<void()>> sentencia_map;
  std::set<std::string> tipo;
  std::set<std::string> tipo_pipeline;

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

  void sentencias();
  void asignacion();
  void fuenteCampo();
  void campos();
  void campo();
  void parametros();
  void cuerpoEscanear();
  void transformar();
  void listaIDs();
  void pipeline();
  void pipelineOp();
  void expresion();
  void termino();
  void factor();
  void contexto();
  void listaContextos();
  void metadato();
  void sino();
  void contextoOId();

  // funciones de control
  void advance();
  bool match(const std::string &type);

public:
  Parser(const List &tokenList);
  void parse();
};
