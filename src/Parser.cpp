#include "Parser.hpp"
#include <iostream>

Parser::Parser(const List &tokenList)
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
      {"identificador", [this]() { asignacion(); }} 
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
  std::cout << current->getToken() << std::endl;
  if (current == nullptr || match("EOF"))
    return;

  std::string tipo = current->getTokenType();

  if (declaracion_map.contains(tipo)) {
    declaracion();
    statements();
  } else if (sentencia_map.contains(tipo)) {
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
void Parser::sentencia() { sentencia_map[current->getTokenType()](); }

void Parser::sentencias() {
  std::cout << current->getToken() << std::endl;
  if (sentencia_map.contains(current->getTokenType())) {
    sentencia_map[current->getTokenType()]();
    advance();
    sentencias();
  }
}

void Parser::defEsquema() {
  std::cout << current->getToken() << std::endl;
  advance();
  if (!match("identificador")) {
    std::cerr << "Error: se esperaba identificador despues de 'esquema'\n";
    return;
  }
  advance();
  if (!match("abre_llave")) {
    std::cerr << "Error: se esperaba '{' en definicion de esquema\n";
    return;
  }
  advance();
  campos();
  if (!match("cierra_llave")) {
    std::cerr << "Error: se esperaba '}' al cerrar esquema\n";
    return;
  }
  advance();
}

void Parser::campos() {
  std::cout << current->getToken() << std::endl;
  campo();
  while (match("coma")) {
    advance();
    campo();
  }
}

void Parser::campo() {
  std::cout << current->getToken() << std::endl;
  std::string tipo = current->getTokenType();
  if (tipo != "caracter" && tipo != "texto" && tipo != "entero" &&
      tipo != "flotante" && tipo != "booleano" && tipo != "lista") {
    std::cerr << "Error: se esperaba tipo campo, se encontro: '" << tipo
              << "'\n";
    return;
  }
  advance();
  if (!match("identificador")) {
    std::cerr << "Error: se esperaba identificador despues del tipo\n";
    return;
  }
  advance();
  if (match("=")) {
    advance();
    fuenteCampo();
  }
}

void Parser::fuenteCampo() {
  std::cout << current->getToken() << std::endl;
  if (current->getTokenType() == "columna") {
    advance();
    if (!match("abre_parentesis")) {
      std::cerr << "Error: se esperaba '(' despues de 'columna'\n";
      return;
    }
    advance();
    if (!match("numero_entero")) {
      std::cerr << "Error: se esperaba numero en 'columna'\n";
      return;
    }
    advance();
    if (!match("cierra_parentesis")) {
      std::cerr << "Error: se esperaba ')' al cerrar 'columna'\n";
      return;
    }
    advance();
  } else if (current->getTokenType() == "regex") {
    advance();
    if (!match("abre_parentesis")) {
      std::cerr << "Error: se esperaba '(' despues de 'regex'\n";
      return;
    }
    advance();
    if (!match("literal_cadena")) {
      std::cerr << "Error: se esperaba cadena en 'regex'\n";
      return;
    }
    advance();
    if (!match("cierra_parentesis")) {
      std::cerr << "Error: se esperaba ')' al cerrar 'regex'\n";
      return;
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
  std::cout << current->getToken() << std::endl;
  advance();
  if (!match("identificador")) {
    std::cerr << "Error: se esperaba identificador despues de 'funcion'\n";
    return;
  }
  advance();
  if (!match("abre_parentesis")) {
    std::cerr << "Error: se esperaba '(' en definicion de funcion\n";
    return;
  }
  advance();
  if (match("identificador")) {
    parametros();
  }
  if (!match("cierra_parentesis")) {
    std::cerr << "Error: se esperaba ')' en definicion de funcion\n";
    return;
  }
  advance();
  if (!match("abre_llave")) {
    std::cerr << "Error: se esperaba '{' en cuerpo de funcion\n";
    return;
  }
  advance();
  sentencias();
  if (!match("cierra_llave")) {
    std::cerr << "Error: se esperaba '}' al cerrar funcion\n";
    return;
  }
  advance();
}

void Parser::parametros() {
  advance();
  while (match("coma")) {
    advance();
    if (!match("identificador")) {
      std::cerr << "Error: se esperaba identificador despues de ','\n";
      return;
    }
    advance();
  }
}

void Parser::asignacionFuente() {
  advance();
  if (!match("identificador")) {
    std::cerr << "Error: se esperaba identificador despues de 'fuente'\n";
    return;
  }
  advance();
  if (!match("=")) {
    std::cerr << "Error: se esperaba '=' en asignacion de fuente\n";
    return;
  }
  advance();
  if (!match("abrir")) {
    std::cerr << "Error: se esperaba 'abrir' en asignacion de fuente\n";
    return;
  }
  advance();
  if (!match("abre_parentesis")) {
    std::cerr << "Error: se esperaba '(' despues de 'abrir'\n";
    return;
  }
  advance();
  if (!match("literal_cadena")) {
    std::cerr << "Error: se esperaba cadena en 'abrir'\n";
    return;
  }
  advance();
  if (!match("cierra_parentesis")) {
    std::cerr << "Error: se esperaba ')' al cerrar 'abrir'\n";
    return;
  }
  advance();
}

void Parser::asignacion() {
  advance();
  if (!match("=")) {
    std::cerr << "Error: se esperaba '=' en asignacion\n";
    return;
  }
  advance();
  expresion();
}

void Parser::condicional() {
  advance();
  expresion();
  if (current->getTokenType() != "entonces") {
    std::cerr << "Error: se esperaba 'entonces' despues de condicion\n";
    return;
  }
  advance();
  if (!match("abre_llave")) {
    std::cerr << "Error: se esperaba '{' en condicional\n";
    return;
  }
  advance();
  sentencias();
  if (!match("cierra_llave")) {
    std::cerr << "Error: se esperaba '}' al cerrar condicional\n";
    return;
  }
  advance();
  sino();
}

void Parser::sino() {
  if (current->getTokenType() == "sino") {
    advance();
    if (!match("abre_llave")) {
      std::cerr << "Error: se esperaba '{' despues de 'sino'\n";
      return;
    }
    advance();
    sentencias();
    if (!match("cierra_llave")) {
      std::cerr << "Error: se esperaba '}' al cerrar 'sino'\n";
      return;
    }
    advance();
  } else if (current->getTokenType() == "entonces") {
    advance();
    if (current->getTokenType() != "si") {
      std::cerr << "Error: se esperaba 'si' despues de 'entonces'\n";
      return;
    }
    advance();
    expresion();
    if (current->getTokenType() != "entonces") {
      std::cerr << "Error: se esperaba 'entonces' despues de condicion\n";
      return;
    }
    advance();
    if (!match("abre_llave")) {
      std::cerr << "Error: se esperaba '{'\n";
      return;
    }
    advance();
    sentencias();
    if (!match("cierra_llave")) {
      std::cerr << "Error: se esperaba '}'\n";
      return;
    }
    advance();
    sino();
  }
}

void Parser::escanear() {
  advance();
  if (!match("abre_parentesis")) {
    std::cerr << "Error: se esperaba '(' despues de 'escanear'\n";
    return;
  }
  advance();
  if (!match("identificador")) {
    std::cerr << "Error: se esperaba identificador en 'escanear'\n";
    return;
  }
  advance();
  if (!match("::")) {
    std::cerr << "Error: se esperaba '::' en 'escanear'\n";
    return;
  }
  advance();
  if (!match("identificador")) {
    std::cerr << "Error: se esperaba identificador despues de '::'\n";
    return;
  }
  advance();
  if (!match("cierra_parentesis")) {
    std::cerr << "Error: se esperaba ')' al cerrar 'escanear'\n";
    return;
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
      return;
    }
    advance();
    contextoOId();
    if (!match("cierra_parentesis")) {
      std::cerr << "Error: se esperaba ')' al cerrar 'frecuencia'\n";
      return;
    }
    advance();
  } else if (tipo == "agrupar") {
    advance();
    if (current->getTokenType() != "por") {
      std::cerr << "Error: se esperaba 'por' despues de 'agrupar'\n";
      return;
    }
    advance();
    contextoOId();
  } else if (tipo == "ordenar") {
    advance();
    if (current->getTokenType() != "por") {
      std::cerr << "Error: se esperaba 'por' despues de 'ordenar'\n";
      return;
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
      return;
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
    return;
  }
  advance();
  if (!match("=>")) {
    std::cerr << "Error: se esperaba '=>' en exportar\n";
    return;
  }
  advance();
  if (!match("literal_cadena")) {
    std::cerr << "Error: se esperaba cadena en exportar\n";
    return;
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
      return;
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
    return;
  }
  advance();
}
