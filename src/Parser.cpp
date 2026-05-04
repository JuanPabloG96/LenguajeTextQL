#include "Parser.hpp"
#include <iostream>

Parser::Parser(const List &tokenList) : head(tokenList) {
  current = head.getHead();
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

bool Parser::match(const std::string &type) {
  return current != nullptr && current->getTokenType() == type;
}

void Parser::advance() {
  if (current != nullptr && !match("EOF"))
    current = current->getNext();
}

void Parser::parse() { statements(); }

void Parser::statements() {
  if (current == nullptr || match("EOF"))
    return;

  std::string tipo = current->getTokenType();

  if (declaracion_map.contains(tipo)) {
    declaracion();
    statements();
  } else if (sentencia_map.contains(tipo) || tipo.contains(tipo)) {
    sentencia();
    statements();
  } else {
    std::cerr << "Error: token inesperado: '" << current->getTokenType()
              << "'\n";
    advance();
    statements();
  }
}

void Parser::declaracion() { declaracion_map[current->getTokenType()](); }
void Parser::sentencia() {
  std::string tipo = current->getTokenType();
  if (tipo.contains(tipo)) {
    asignacion();
  } else
    sentencia_map[current->getTokenType()]();
}

void Parser::sentencias() {
  if (sentencia_map.contains(current->getTokenType()) ||
      tipo.contains(current->getTokenType())) {
    if (tipo.contains(current->getTokenType()))
      asignacion();
    else
      sentencia_map[current->getTokenType()]();
    advance();
    sentencias();
  }
}

void Parser::defEsquema() {
  advance();
  if (!match("identificador")) {
    std::cerr << "Error: se esperaba identificador despues de 'esquema'\n";
  }
  advance();
  if (!match("abre_llave")) {
    std::cerr << "Error: se esperaba '{' en definicion de esquema\n";
  }
  advance();
  campos();
  if (!match("cierra_llave")) {
    std::cerr << "Error: se esperaba '}' al cerrar esquema\n";
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
  std::string tipo = current->getTokenType();
  if (tipo != "caracter" && tipo != "texto" && tipo != "entero" &&
      tipo != "flotante" && tipo != "booleano" && tipo != "lista") {
    std::cerr << "Error: se esperaba tipo campo, se encontro: '" << tipo
              << "'\n";
  }
  advance();
  if (!match("identificador")) {
    std::cerr << "Error: se esperaba identificador despues del tipo\n";
  }
  advance();
  if (match("=")) {
    advance();
    fuenteCampo();
  }
}

void Parser::fuenteCampo() {
  if (current->getTokenType() == "columna") {
    advance();
    if (!match("abre_parentesis")) {
      std::cerr << "Error: se esperaba '(' despues de 'columna'\n";
    }
    advance();
    if (!match("numero_entero")) {
      std::cerr << "Error: se esperaba numero en 'columna'\n";
    }
    advance();
    if (!match("cierra_parentesis")) {
      std::cerr << "Error: se esperaba ')' al cerrar 'columna'\n";
    }
    advance();
  } else if (current->getTokenType() == "regex") {
    advance();
    if (!match("abre_parentesis")) {
      std::cerr << "Error: se esperaba '(' despues de 'regex'\n";
    }
    advance();
    if (!match("literal_cadena")) {
      std::cerr << "Error: se esperaba cadena en 'regex'\n";
    }
    advance();
    if (!match("cierra_parentesis")) {
      std::cerr << "Error: se esperaba ')' al cerrar 'regex'\n";
    }
    advance();
  } else if (match("@")) {
    metadato();
  } else {
    std::cerr << "Error: se esperaba 'columna', 'regex' o metadato en fuente "
                 "de campo\n";
  }
}

void Parser::defFuncion() {
  advance();
  if (!match("identificador")) {
    std::cerr << "Error: se esperaba identificador despues de 'funcion'\n";
  }
  advance();
  if (!match("abre_parentesis")) {
    std::cerr << "Error: se esperaba '(' en definicion de funcion\n";
  }
  advance();
  if (match("identificador")) {
    parametros();
  }
  if (!match("cierra_parentesis")) {
    std::cerr << "Error: se esperaba ')' en definicion de funcion\n";
  }
  advance();
  if (!match("abre_llave")) {
    std::cerr << "Error: se esperaba '{' en cuerpo de funcion\n";
  }
  advance();
  sentencias();
  if (!match("cierra_llave")) {
    std::cerr << "Error: se esperaba '}' al cerrar funcion\n";
  }
  advance();
}

void Parser::parametros() {
  advance();
  while (match("coma")) {
    advance();
    if (!match("identificador")) {
      std::cerr << "Error: se esperaba identificador despues de ','\n";
    }
    advance();
  }
}

void Parser::asignacionFuente() {
  advance();
  if (!match("identificador")) {
    std::cerr << "Error: se esperaba identificador despues de 'fuente'\n";
  }
  advance();
  if (!match("=")) {
    std::cerr << "Error: se esperaba '=' en asignacion de fuente\n";
  }
  advance();
  if (!match("abrir")) {
    std::cerr << "Error: se esperaba 'abrir' en asignacion de fuente\n";
  }
  advance();
  if (!match("abre_parentesis")) {
    std::cerr << "Error: se esperaba '(' despues de 'abrir'\n";
  }
  advance();
  if (!match("literal_cadena")) {
    std::cerr << "Error: se esperaba cadena en 'abrir'\n";
  }
  advance();
  if (!match("cierra_parentesis")) {
    std::cerr << "Error: se esperaba ')' al cerrar 'abrir'\n";
  }
  advance();
}

void Parser::asignacion() {
  std::string tipo = current->getTokenType();
  if (tipo.contains(tipo)) {
    advance();
  }
  if (!match("identificador")) {
    std::cerr << "Error se esperaba un identificador en asignacion"
              << std::endl;
  }
  advance();
  if (!match("=")) {
    std::cerr << "Error: se esperaba '=' en asignacion\n";
  }
  advance();
  expresion();
}

void Parser::condicional() {
  advance();
  expresion();
  if (current->getTokenType() != "entonces") {
    std::cerr << "Error: se esperaba 'entonces' despues de condicion\n";
  }
  advance();
  if (!match("abre_llave")) {
    std::cerr << "Error: se esperaba '{' en condicional\n";
  }
  advance();
  sentencias();
  if (!match("cierra_llave")) {
    std::cerr << "Error: se esperaba '}' al cerrar condicional\n";
  }
  advance();
  sino();
}

void Parser::sino() {
  if (current->getTokenType() == "sino") {
    advance();
    if (!match("abre_llave")) {
      std::cerr << "Error: se esperaba '{' despues de 'sino'\n";
    }
    advance();
    sentencias();
    if (!match("cierra_llave")) {
      std::cerr << "Error: se esperaba '}' al cerrar 'sino'\n";
    }
    advance();
  } else if (current->getTokenType() == "entonces") {
    advance();
    if (current->getTokenType() != "si") {
      std::cerr << "Error: se esperaba 'si' despues de 'entonces'\n";
    }
    advance();
    expresion();
    if (current->getTokenType() != "entonces") {
      std::cerr << "Error: se esperaba 'entonces' despues de condicion\n";
    }
    advance();
    if (!match("abre_llave")) {
      std::cerr << "Error: se esperaba '{'\n";
    }
    advance();
    sentencias();
    if (!match("cierra_llave")) {
      std::cerr << "Error: se esperaba '}'\n";
    }
    advance();
    sino();
  }
}

void Parser::escanear() {
  advance();
  if (!match("abre_parentesis")) {
    std::cerr << "Error: se esperaba '(' despues de 'escanear'\n";
  }
  advance();
  if (!match("identificador")) {
    std::cerr << "Error: se esperaba identificador en 'escanear'\n";
  }
  advance();
  if (!match("::")) {
    std::cerr << "Error: se esperaba '::' en 'escanear'\n";
  }
  advance();
  if (!match("identificador")) {
    std::cerr << "Error: se esperaba identificador despues de '::'\n";
  }
  advance();
  if (!match("cierra_parentesis")) {
    std::cerr << "Error: se esperaba ')' al cerrar 'escanear'\n";
  }
  advance();
  cuerpoEscanear();
}

void Parser::cuerpoEscanear() {
  if (current->getTokenType() == "donde") {
    advance();
    expresion();
  }
  if (match("->")) {
    transformar();
  }
}

void Parser::transformar() {
  advance();
  listaContextos();
  std::string tipo = current->getTokenType();
  if (tipo_pipeline.contains(tipo))
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
  if (match("$")) {
    contexto();
  } else if (match("identificador")) {
    advance();
  } else {
    std::cerr << "Error: se esperaba contexto o identificador\n";
  }
}

void Parser::contexto() {
  advance();
  if (!match("identificador")) {
    std::cerr << "Error se esperaba identificador despues de contexto\n";
  }
  advance();
}

void Parser::pipeline() {
  std::string tipo = current->getTokenType();
  while (tipo_pipeline.contains(tipo)) {
    pipelineOp();
    tipo = current->getTokenType();
  }
}

void Parser::pipelineOp() {
  std::string tipo = current->getTokenType();
  if (tipo == "frecuencia") {
    advance();
    if (!match("abre_parentesis")) {
      std::cerr << "Error: se esperaba '(' despues de 'frecuencia'\n";
    }
    advance();
    contextoOId();
    if (!match("cierra_parentesis")) {
      std::cerr << "Error: se esperaba ')' al cerrar 'frecuencia'\n";
    }
    advance();
  } else if (tipo == "agrupar") {
    advance();
    if (current->getTokenType() != "por") {
      std::cerr << "Error: se esperaba 'por' despues de 'agrupar'\n";
    }
    advance();
    contextoOId();
  } else if (tipo == "ordenar") {
    advance();
    if (current->getTokenType() != "por") {
      std::cerr << "Error: se esperaba 'por' despues de 'ordenar'\n";
    }
    advance();
    contextoOId();
    if (current->getTokenType() == "asc" || current->getTokenType() == "desc") {
      advance();
    }
  } else if (tipo == "normalizar") {
    advance();
  } else if (tipo == "puntuacion") {
    advance();
  } else if (tipo == "limite") {
    advance();
    if (!match("numero_entero")) {
      std::cerr << "Error: se esperaba numero despues de 'limite'\n";
    }
    advance();
  }
}

void Parser::mostrar() {
  advance();
  expresion();
}

void Parser::exportar() {
  advance();
  if (!match("identificador")) {
    std::cerr << "Error: se esperaba identificador despues de 'exportar'\n";
  }
  advance();
  if (!match("=>")) {
    std::cerr << "Error: se esperaba '=>' en exportar\n";
  }
  advance();
  if (!match("literal_cadena")) {
    std::cerr << "Error: se esperaba cadena en exportar\n";
  }
  advance();
}

void Parser::retorno() {
  advance();
  expresion();
}

void Parser::expresion() {
  termino();
  while (match("+") || match("-") || match("==") || match("||") ||
         match("&&")) {
    advance();
    termino();
  }
}

void Parser::termino() {
  factor();
  while (match("*") || match("/") || match("~~") || match("::")) {
    advance();
    factor();
  }
}

void Parser::factor() {
  if (match("abre_parentesis")) {
    advance();
    expresion();
    if (!match("cierra_parentesis")) {
      std::cerr << "Error: se esperaba ')' al cerrar expresion\n";
    }
    advance();
  } else if (match("numero_entero") || match("numero_real")) {
    advance();
  } else if (match("literal_cadena") || match("literal_caracter")) {
    advance();
  } else if (match("$")) {
    contexto();
  } else if (match("@")) {
    metadato();
  } else if (match("identificador")) {
    advance();
  } else if (match("escanear")) {
    escanear();
  } else {
    std::cerr << "Error: factor invalido: '" << current->getTokenType()
              << "'\n";
    advance();
  }
}

void Parser::metadato() {
  advance();
  if (!match("identificador")) {
    std::cerr << "Error: se esperaba identificador despues de '@'\n";
  }
  advance();
}
