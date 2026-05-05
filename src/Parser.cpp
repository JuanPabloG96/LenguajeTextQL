#include "Parser.hpp"

#include <cstdlib>
#include <iostream>

Parser::Parser(const List& tokenList) : head(tokenList) {
  current = head.getHead();
  // clang-format off
  declaracion_map = {
      {"esquema", [this]() { defEsquema(); }},
      {"funcion",  [this]() { defFuncion(); }},
      {"fuente",   [this]() { asignacionFuente(); }},
  };
  sentencia_map = {
      {"si",       [this]() { condicional(); }},
      {"mostrar",  [this]() { mostrar(); }},
      {"exportar", [this]() { exportar(); }},
      {"retornar", [this]() { retorno(); }},
      {"identificador", [this]() { asignacion(); }}
  };
  tipo = {
    "caracter",
    "texto", 
    "entero",
    "flotante",
    "booleano",
    "lista"
  };
  tipo_pipeline = {
    "frecuencia",
    "agrupar",
    "ordenar",
    "normalizar",
    "puntuacion",
    "limite"
  };
  // clang-format on
}

// funciones auxiliares
bool Parser::match(const std::string& type) {
  return current != nullptr && current->getTokenType() == type;
}
void Parser::advance() {
  if (current != nullptr && !match("EOF"))
    current = current->getNext();
  std::cout << "token actual: " << current->getToken() << std::endl;
}
void Parser::actualizar_tipo() {
  tipo_actual = current->getTokenType();
}
void Parser::error(std::string esperado,
                   std::string funcion,
                   std::string tipo = "") {
  std::cerr << "(" << funcion << ") " << "Error, se esperaba " << tipo << " '"
            << esperado << "', en linea " << current->getLine() << " columna "
            << current->getColumn() << ", Token: " << current->getToken()
            << std::endl;
  std::exit(0);
}

// funciones del parser
void Parser::parse() {
  statements();
}

void Parser::statements() {
  if (current == nullptr || match("EOF"))
    return;

  actualizar_tipo();

  if (declaracion_map.contains(tipo_actual)) {
    declaracion();
    statements();
  } else if (sentencia_map.contains(tipo_actual) ||
             tipo.contains(tipo_actual)) {
    sentencia();
    statements();
  } else {
    std::cerr << "Error de token no reconocido, en linea " << current->getLine()
              << " columna " << current->getColumn()
              << ", Token: " << current->getToken() << std::endl;
  }
}

void Parser::declaracion() {
  actualizar_tipo();
  declaracion_map[tipo_actual]();
}

void Parser::asignacionFuente() {
  if (!match("fuente")) {
    error("fuente", "asignacionFuente", "palabra reservada");
  }
  advance();
  if (!match("identificador")) {
    error("identificador", "asignacionFuente");
  }
  advance();
  if (!match("=")) {
    error("=", "asignacionFuente", "simbolo");
  }
  advance();
  if (!match("abrir")) {
    error("abrir", "asignacionFuente", "palabra reservada");
  }
  advance();
  if (!match("abre_parentesis")) {
    error("(", "asignacionFuente", "simbolo");
  }
  advance();
  if (!match("literal_cadena")) {
    error("literal cadena", "asignacionFuente", "tipo de dato");
  }
  advance();
  if (!match("cierra_parentesis")) {
    error(")", "asignacionFuente", "simbolo");
  }
  advance();
}

void Parser::defEsquema() {
  if (!match("esquema")) {
    error("esquema", "defEsquema", "palabra reservada");
  }
  advance();
  if (!match("identificador")) {
    error("identificador", "defEsquema");
  }
  advance();
  if (!match("abre_llave")) {
    error("{", "defEsquema", "simbolo");
  }
  advance();
  campos();
  if (!match("cierra_llave")) {
    error("}", "defEsquema", "simbolo");
  }
  advance();
}

void Parser::campos() {
  campo();
  while (match("coma")) {
    advance();
    campo();
  }
}

void Parser::campo() {
  actualizar_tipo();
  if (!tipo.contains(tipo_actual)) {
    error("tipo de dato", "campo");
  }
  advance();
  if (!match("identificador")) {
    error("identificador", "campo");
  }
  advance();
  if (match("=")) {
    advance();
    fuenteCampo();
  }
}

void Parser::fuenteCampo() {
  if (match("columna")) {
    advance();
    if (!match("abre_parentesis")) {
      error("(", "fuenteCampo", "simbolo");
    }
    advance();
    if (!match("numero_entero")) {
      error("numero entero", "fuenteCampo", "tipo de dato");
    }
    advance();
    if (!match("cierra_parentesis")) {
      error(")", "fuenteCampo", "simbolo");
    }
    advance();
  } else if (match("@")) {
    metadato();
  }
}

void Parser::defFuncion() {
  if (!match("funcion"))
    error("funcion", "defFuncion", "palabra reservada");
  advance();
  if (!match("identificador"))
    error("identificador", "defFuncion");
  advance();
  if (!match("abre_parentesis"))
    error("(", "defFuncion", "simbolo");
  advance();
  if (match("identificador"))
    parametros();
  if (!match("cierra_parentesis"))
    error(")", "defFuncion", "simbolo");
  advance();
  if (!match("abre_llave"))
    error("{", "defFuncion", "simbolo");
  advance();
  sentencias();
  if (!match("cierra_llave"))
    error("}", "defFuncion", "simbolo");
  advance();
}

void Parser::parametros() {
  if (!match("identificador"))
    error("identificador", "parametros");
  advance();
  while (match("coma")) {
    advance();
    if (!match("identificador"))
      error("identificador", "parametros");
    advance();
  }
}
void Parser::sentencias() {
  actualizar_tipo();
  while (sentencia_map.contains(tipo_actual) || tipo.contains(tipo_actual)) {
    sentencia();
    actualizar_tipo();
  }
}

void Parser::sentencia() {
  if (tipo.contains(tipo_actual))
    asignacion();
  else
    sentencia_map[tipo_actual]();
}

void Parser::asignacion() {
  if (tipo.contains(tipo_actual))
    declaracionVar();
  else if (match("identificador"))
    reasignacion();
  else
    error("asignacion", "asignacion");
}
void Parser::declaracionVar() {
  if (!tipo.contains(tipo_actual))
    error("tipo de dato", "declaracionVar");
  advance();
  if (!match("identificador"))
    error("identificador", "declaracionVar");
  advance();
  if (!match("="))
    error("=", "simbolo");
  advance();
  expresion();
}
void Parser::reasignacion() {
  if (!match("identificador"))
    error("identificador", "reasignacion");
  advance();
  if (!match("="))
    error("=", "simbolo");
  advance();
  expresion();
}
void Parser::condicional() {
  if (!match("si"))
    error("si", "condicional", "palabra reservada");
  advance();
  expresion();
  if (!match("entonces"))
    error("entonces", "condicional", "palabra reservada");
  advance();
  if (!match("abre_llave"))
    error("{", "condicional", "simbolo");
  advance();
  sentencias();
  if (!match("cierra_llave"))
    error("}", "condicional", "simbolo");
  advance();
  sino();
}
void Parser::sino() {
  if (match("entonces")) {
    advance();
    if (!match("si"))
      error("si", "sino", "palabra reservada");
    advance();
    expresion();
    if (!match("entonces"))
      error("entonces", "sino", "palabra reservada");
    advance();
    if (!match("abre_llave"))
      error("{", "sino", "simbolo");
    advance();
    sentencias();
    if (!match("cierra_llave"))
      error("}", "sino", "simbolo");
    advance();
    sino();
  }
  if (match("sino")) {
    advance();
    if (!match("abre_llave"))
      error("{", "sino", "simbolo");
    advance();
    sentencias();
    if (!match("cierra_llave"))
      error("}", "sino", "simbolo");
    advance();
  }
}
void Parser::expresion() {
  term();
  expresionPrime();
}
void Parser::expresionPrime() {
  if (match("+") || match("-") || match("==") || match("||") || match("&&")) {
    advance();
    term();
    expresionPrime();
  }
}
void Parser::term() {
  factor();
  termPrime();
}
void Parser::termPrime() {
  if (match("*") || match("/") || match("~~") || match("::")) {
    factor();
    termPrime();
  }
}
void Parser::factor() {
  if (match("abre_parentesis")) {
    expresion();
    if (!match("cierra_parentesis"))
      error(")", "factor", "simbolo");
  } else if (match("$"))
    contexto();
  else if (match("@"))
    metadato();
  else if (match("escanear"))
    escanearExpr();
  else if (match("numero_entero") || match("numero_flotante") ||
           match("literal_cadena") || match("identificador"))
    advance();
  else
    error("tipo factor", "factor");
}
void Parser::escanearExpr() {
  if (!match("escanear"))
    error("escanear", "escanearExpr", "palabra reservada");
  advance();
  if (!match("abre_parentesis"))
    error("(", "escanearExpr", "simbolo");
  advance();
  if (!match("identificador"))
    error("identificador", "escanearExpr");
  advance();
  if (!match("::"))
    error("::", "escanearExpr", "simbolo");
  advance();
  if (!match("identificador"))
    error("identificador", "escanearExpr");
  advance();
  if (!match("cierra_parentesis"))
    error(")", "escanearExpr", "simbolo");
  advance();
  cuerpoEscanear();
}
void Parser::cuerpoEscanear() {
  if (match("donde"))
    donde();
  if (match("->"))
    transformar();
}
void Parser::donde() {
  if (!match("donde"))
    error("donde", "donde", "palabra reservada");
  advance();
  expresion();
}
void Parser::transformar() {
  if (!match("->"))
    error("->", "transformar", "simbolo");
  advance();
  listaContextos();
  actualizar_tipo();
  if (tipo_pipeline.contains(tipo_actual))
    pipeline();
}
void Parser::listaContextos() {
  contextoOId();
  while (match("coma")) {
    advance();
    contextoOId();
  }
}
void Parser::contextoOId() {
  if (match("$"))
    contexto();
  else if (match("identificador"))
    advance();
  else
    error("contexto o identificador", "contextoOId");
}
void Parser::pipeline() {
  actualizar_tipo();
  while (tipo_pipeline.contains(tipo_actual)) {
    pipelineOp();
  }
}
void Parser::pipelineOp() {
  if (match("frecuencia")) {
    advance();
    if (!match("abre_parentesis"))
      error("(", "pipelineOp", "simbolo");
    advance();
    contextoOId();
    if (!match("cierra_parentesis"))
      error(")", "pipelineOp", "simbolo");
    advance();
  }
  if (match("agrupar")) {
    advance();
    if (!match("por"))
      error("por", "pipelineOp", "palabra reservada");
    advance();
    contextoOId();
  }
  if (match("ordenar")) {
    advance();
    if (!match("por"))
      error("por", "pipelineOp", "palabra reservada");
    advance();
    if (match("asc") || match("desc"))
      advance();
  }
  if (match("limite")) {
    advance();
    if (!match("numero_entero"))
      error("numero entero", "pipelineOp", "tipo de dato");
    advance();
  }
  if (match("normalizar") || match("puntuacion"))
    advance();
}
void Parser::contexto() {
  if (!match("$"))
    error("$", "contexto", "simbolo");
  advance();
  if (!match("identificador"))
    error("identificador", "contexto");
  advance();
}
void Parser::metadato() {
  if (!match("@"))
    error("@", "metadato", "simbolo");
  advance();
  if (!match("identificador"))
    error("identificador", "metadato");
  advance();
}
void Parser::mostrar() {
  if (!match("mostrar"))
    error("mostrar", "mostrar", "palabra reservada");
  advance();
  expresion();
}
void Parser::exportar() {
  if (!match("exportar"))
    error("exportar", "exportar", "palabra reservada");
  advance();
  if (!match("identificador"))
    error("identificador", "exportar");
  advance();
  if (!match("=>"))
    error("=>", "exportar", "simbolo");
  advance();
  if (!match("literal_cadena"))
    error("cadena de caracteres", "exportar", "tipo de dato");
  advance();
}
void Parser::retorno() {
  if (!match("retornar"))
    error("retornar", "retorno", "palabra reservada");
  expresion();
}
