#pragma once

#include <functional>
#include <set>
#include <unordered_map>

#include "List.hpp"
#include "NodoAST.hpp"

class Parser {
 private:
  const List& head;
  Node* current;
  std::string tipo_actual;

  std::unordered_map<std::string, std::function<void()>> declaracion_map;
  std::unordered_map<std::string, std::function<void()>> sentencia_map;
  std::set<std::string> tipo;
  std::set<std::string> tipo_pipeline;

  // funciones del parser
  void statements();
  void declaracion();
  void asignacionFuente();
  void defEsquema();
  void campos();
  void campo();
  void fuenteCampo();
  void defFuncion();
  void parametros();
  void sentencias();
  void sentencia();
  void asignacion();
  void declaracionVar();
  void reasignacion();
  void condicional();
  void sino();
  void expresion();
  void expresionPrime();
  void term();
  void termPrime();
  void factor();
  void escanearExpr();
  void cuerpoEscanear();
  void donde();
  void transformar();
  void listaContextos();
  void contextoOId();
  void pipeline();
  void pipelineOp();
  void contexto();
  void metadato();
  void mostrar();
  void exportar();
  void retorno();

  // funciones de control
  void advance();
  bool match(const std::string& type);
  void actualizar_tipo();

  // mostrar errores
  void error(std::string esperado, std::string funcion, std::string tipo);

  // parametros y funciones para arbol AST
  std::shared_ptr<NodoAST> raiz;
  std::shared_ptr<NodoAST> nodoActual;

  void pushNodo(const std::string& tipo, const std::string& valor = "");
  void popNodo(std::shared_ptr<NodoAST> padre);

 public:
  Parser(const List& tokenList);
  void parse();
  // funcion para arbol AST
  std::shared_ptr<NodoAST> getRaiz() const;
};
