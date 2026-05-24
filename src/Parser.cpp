#include "Parser.hpp"

#include <cstdlib>
#include <iostream>

Parser::Parser(const List& tokenList) : head(tokenList)
{
    current = head.getHead();
    raiz = std::make_shared<NodoAST>("statements");
    nodoActual = raiz;
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
bool Parser::match(const std::string& type)
{
    return current != nullptr && current->getTokenType() == type;
}
void Parser::advance()
{
    // std::cout << "token actual: " << current->getToken() << std::endl;
    if (current != nullptr && !match("EOF"))
        current = current->getNext();
}
void Parser::actualizar_tipo()
{
    tipo_actual = current->getTokenType();
}
void Parser::error(std::string esperado,
                   std::string funcion,
                   std::string tipo = "")
{
    std::cerr << "(" << funcion << ") " << "Error, se esperaba " << tipo << " '"
              << esperado << "', en linea " << current->getLine() << " columna "
              << current->getColumn() << ", Token: " << current->getToken()
              << std::endl;
    std::exit(0);
}

// funciones del arbol AST
void Parser::pushNodo(const std::string& tipo, const std::string& valor)
{
    auto nuevo = nodoActual->crearHijo(tipo, valor);
    nodoActual = nuevo;
}

void Parser::popNodo(std::shared_ptr<NodoAST> padre)
{
    nodoActual = padre;
}

std::shared_ptr<NodoAST> Parser::getRaiz() const
{
    return this->raiz;
}

// funciones del parser
void Parser::parse()
{
    statements();
}

void Parser::statements()
{
    if (current == nullptr || match("EOF"))
        return;

    actualizar_tipo();

    if (declaracion_map.contains(tipo_actual))
    {
        declaracion();
        statements();
    }
    else if (sentencia_map.contains(tipo_actual) || tipo.contains(tipo_actual))
    {
        sentencia();
        statements();
    }
    else
    {
        std::cerr << "Error de token no reconocido, en linea "
                  << current->getLine() << " columna " << current->getColumn()
                  << ", Token: " << current->getToken() << std::endl;
    }
}

void Parser::declaracion()
{
    actualizar_tipo();
    declaracion_map[tipo_actual]();
}

void Parser::asignacionFuente()
{
    auto padre = nodoActual;
    pushNodo("asignacionFuente");

    if (!match("fuente"))
        error("fuente", "asignacionFuente", "palabra reservada");
    advance();

    if (!match("identificador"))
        error("identificador", "asignacionFuente");
    pushNodo("identificador", current->getToken());
    popNodo(nodoActual->getParent()); // volver a asignacionFuente
    advance();

    if (!match("="))
        error("=", "asignacionFuente", "simbolo");
    advance();

    if (!match("abrir"))
        error("abrir", "asignacionFuente", "palabra reservada");
    advance();

    if (!match("abre_parentesis"))
        error("(", "asignacionFuente", "simbolo");
    advance();

    if (!match("literal_cadena"))
        error("literal cadena", "asignacionFuente", "tipo de dato");
    pushNodo("ruta", current->getToken());
    popNodo(nodoActual->getParent()); // volver a asignacionFuente
    advance();

    if (!match("cierra_parentesis"))
        error(")", "asignacionFuente", "simbolo");
    advance();

    popNodo(padre);
}

void Parser::defEsquema()
{
    auto padre = nodoActual;
    pushNodo("defEsquema");

    if (!match("esquema"))
        error("esquema", "defEsquema", "palabra reservada");
    advance();

    if (!match("identificador"))
        error("identificador", "defEsquema");
    pushNodo("nombre", current->getToken());
    popNodo(nodoActual->getParent()); // volver a defEsquema
    advance();

    if (!match("abre_llave"))
        error("{", "defEsquema", "simbolo");
    advance();

    campos();

    if (!match("cierra_llave"))
        error("}", "defEsquema", "simbolo");
    advance();

    popNodo(padre);
}

void Parser::campos()
{
    auto padre = nodoActual;
    pushNodo("campos");

    campo();
    while (match("coma"))
    {
        advance();
        campo();
    }

    popNodo(padre);
}

void Parser::campo()
{
    auto padre = nodoActual;
    actualizar_tipo();

    if (!tipo.contains(tipo_actual))
        error("tipo de dato", "campo");

    pushNodo("campo");
    pushNodo("tipo", tipo_actual);
    popNodo(nodoActual->getParent()); // volver a campo
    advance();

    if (!match("identificador"))
        error("identificador", "campo");
    pushNodo("nombre", current->getToken());
    popNodo(nodoActual->getParent()); // volver a campo
    advance();

    if (match("="))
    {
        advance();
        fuenteCampo();
    }

    popNodo(padre);
}

void Parser::fuenteCampo()
{
    auto padre = nodoActual;
    pushNodo("fuenteCampo");

    if (match("columna"))
    {
        advance();
        if (!match("abre_parentesis"))
            error("(", "fuenteCampo", "simbolo");
        advance();
        if (!match("numero_entero"))
            error("numero entero", "fuenteCampo", "tipo de dato");
        pushNodo("columna", current->getToken());
        popNodo(nodoActual->getParent()); // volver a fuenteCampo
        advance();
        if (!match("cierra_parentesis"))
            error(")", "fuenteCampo", "simbolo");
        advance();
    }
    else if (match("@"))
    {
        metadato();
    }

    popNodo(padre);
}

void Parser::defFuncion()
{
    auto padre = nodoActual;
    pushNodo("defFuncion");

    if (!match("funcion"))
        error("funcion", "defFuncion", "palabra reservada");
    advance();

    if (!match("identificador"))
        error("identificador", "defFuncion");
    pushNodo("nombre", current->getToken());
    popNodo(nodoActual->getParent()); // volver a defFuncion
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

    popNodo(padre);
}

void Parser::parametros()
{
    auto padre = nodoActual;
    pushNodo("parametros");

    if (!match("identificador"))
        error("identificador", "parametros");
    pushNodo("param", current->getToken());
    popNodo(nodoActual->getParent()); // volver a parametros
    advance();

    while (match("coma"))
    {
        advance();
        if (!match("identificador"))
            error("identificador", "parametros");
        pushNodo("param", current->getToken());
        popNodo(nodoActual->getParent()); // volver a parametros
        advance();
    }

    popNodo(padre);
}

void Parser::sentencias()
{
    actualizar_tipo();
    if (sentencia_map.contains(tipo_actual) || tipo.contains(tipo_actual))
    {
        sentencia();
        sentencias();
    }
}

void Parser::sentencia()
{
    if (tipo.contains(tipo_actual))
        asignacion();
    else
        sentencia_map[tipo_actual]();
}

void Parser::asignacion()
{
    if (tipo.contains(tipo_actual))
        declaracionVar();
    else if (match("identificador"))
        reasignacion();
    else
        error("asignacion", "asignacion");
}

void Parser::declaracionVar()
{
    auto padre = nodoActual;
    pushNodo("declaracionVar");

    if (!tipo.contains(tipo_actual))
        error("tipo de dato", "declaracionVar");
    pushNodo("tipo", tipo_actual);
    popNodo(nodoActual->getParent()); // volver a declaracionVar
    advance();

    if (!match("identificador"))
        error("identificador", "declaracionVar");
    pushNodo("nombre", current->getToken());
    popNodo(nodoActual->getParent()); // volver a declaracionVar
    advance();

    if (!match("="))
        error("=", "simbolo");
    advance();

    expresion();

    popNodo(padre);
}

void Parser::reasignacion()
{
    auto padre = nodoActual;
    pushNodo("reasignacion");

    if (!match("identificador"))
        error("identificador", "reasignacion");
    pushNodo("nombre", current->getToken());
    popNodo(nodoActual->getParent()); // volver a reasignacion
    advance();

    if (!match("="))
        error("=", "simbolo");
    advance();

    expresion();

    popNodo(padre);
}

void Parser::condicional()
{
    auto padre = nodoActual;
    pushNodo("condicional");

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

    popNodo(padre);
}

void Parser::sino()
{
    if (match("entonces"))
    {
        auto padre = nodoActual;
        pushNodo("sino_si");

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
        popNodo(padre);
    }
    if (match("sino"))
    {
        auto padre = nodoActual;
        pushNodo("sino");

        advance();
        if (!match("abre_llave"))
            error("{", "sino", "simbolo");
        advance();

        sentencias();

        if (!match("cierra_llave"))
            error("}", "sino", "simbolo");
        advance();

        popNodo(padre);
    }
}

void Parser::expresion()
{
    auto padre = nodoActual;
    pushNodo("expresion");

    term();
    expresionPrime();

    popNodo(padre);
}

void Parser::expresionPrime()
{
    if (match("+") || match("-") || match("==") || match("||") || match("&&"))
    {
        auto padre = nodoActual;
        pushNodo("op", current->getToken());
        popNodo(nodoActual->getParent()); // volver al padre de expresionPrime
        advance();
        term();
        expresionPrime();
    }
}

void Parser::term()
{
    factor();
    termPrime();
}

void Parser::termPrime()
{
    if (match("*") || match("/") || match("~~") || match("::"))
    {
        auto padre = nodoActual;
        pushNodo("op", current->getToken());
        popNodo(nodoActual->getParent());
        advance();
        factor();
        termPrime();
    }
}

void Parser::factor()
{
    if (match("abre_parentesis"))
    {
        advance();
        expresion();
        if (!match("cierra_parentesis"))
            error(")", "factor", "simbolo");
        advance();
    }
    else if (match("$"))
        contexto();
    else if (match("@"))
        metadato();
    else if (match("escanear"))
        escanearExpr();
    else if (match("numero_entero") || match("numero_flotante") ||
             match("literal_cadena") || match("identificador"))
    {
        pushNodo(current->getTokenType(), current->getToken());
        popNodo(nodoActual->getParent());
        advance();
    }
    else
        error("tipo factor", "factor");
}

void Parser::escanearExpr()
{
    auto padre = nodoActual;
    pushNodo("escanear");

    if (!match("escanear"))
        error("escanear", "escanearExpr", "palabra reservada");
    advance();

    if (!match("abre_parentesis"))
        error("(", "escanearExpr", "simbolo");
    advance();

    if (!match("identificador"))
        error("identificador", "escanearExpr");
    pushNodo("fuente", current->getToken());
    popNodo(nodoActual->getParent()); // volver a escanear
    advance();

    if (!match("::"))
        error("::", "escanearExpr", "simbolo");
    advance();

    if (!match("identificador"))
        error("identificador", "escanearExpr");
    pushNodo("esquema", current->getToken());
    popNodo(nodoActual->getParent()); // volver a escanear
    advance();

    if (!match("cierra_parentesis"))
        error(")", "escanearExpr", "simbolo");
    advance();

    cuerpoEscanear();

    popNodo(padre);
}

void Parser::cuerpoEscanear()
{
    if (match("donde"))
        donde();
    if (match("->"))
        transformar();
}

void Parser::donde()
{
    auto padre = nodoActual;
    pushNodo("donde");

    if (!match("donde"))
        error("donde", "donde", "palabra reservada");
    advance();

    expresion();

    popNodo(padre);
}

void Parser::transformar()
{
    auto padre = nodoActual;
    pushNodo("transformar");

    if (!match("->"))
        error("->", "transformar", "simbolo");
    advance();

    listaContextos();

    actualizar_tipo();
    if (tipo_pipeline.contains(tipo_actual))
        pipeline();

    popNodo(padre);
}

void Parser::listaContextos()
{
    auto padre = nodoActual;
    pushNodo("listaContextos");

    contextoOId();
    while (match("coma"))
    {
        advance();
        contextoOId();
    }

    popNodo(padre);
}

void Parser::contextoOId()
{
    if (match("$"))
        contexto();
    else if (match("identificador"))
    {
        pushNodo("identificador", current->getToken());
        popNodo(nodoActual->getParent());
        advance();
    }
    else
        error("contexto o identificador", "contextoOId");
}

void Parser::pipeline()
{
    auto padre = nodoActual;
    pushNodo("pipeline");

    actualizar_tipo();
    while (tipo_pipeline.contains(tipo_actual))
    {
        pipelineOp();
        actualizar_tipo();
    }

    popNodo(padre);
}

void Parser::pipelineOp()
{
    auto padre = nodoActual;

    if (match("frecuencia"))
    {
        pushNodo("frecuencia");
        advance();
        if (!match("abre_parentesis"))
            error("(", "pipelineOp", "simbolo");
        advance();
        contextoOId();
        if (!match("cierra_parentesis"))
            error(")", "pipelineOp", "simbolo");
        advance();
    }
    else if (match("agrupar"))
    {
        pushNodo("agrupar");
        advance();
        if (!match("por"))
            error("por", "pipelineOp", "palabra reservada");
        advance();
        contextoOId();
    }
    else if (match("ordenar"))
    {
        pushNodo("ordenar");
        advance();
        if (!match("por"))
            error("por", "pipelineOp", "palabra reservada");
        advance();
        contextoOId();
        if (match("asc") || match("desc"))
        {
            pushNodo("orden", current->getToken());
            popNodo(nodoActual->getParent()); // volver a ordenar
            advance();
        }
    }
    else if (match("limite"))
    {
        pushNodo("limite");
        advance();
        if (!match("numero_entero"))
            error("numero entero", "pipelineOp", "tipo de dato");
        pushNodo("valor", current->getToken());
        popNodo(nodoActual->getParent()); // volver a limite
        advance();
    }
    else if (match("normalizar") || match("puntuacion"))
    {
        pushNodo(current->getTokenType());
        popNodo(nodoActual->getParent());
        advance();
    }

    popNodo(padre);
}

void Parser::contexto()
{
    auto padre = nodoActual;
    pushNodo("contexto");

    if (!match("$"))
        error("$", "contexto", "simbolo");
    advance();

    if (!match("identificador"))
        error("identificador", "contexto");
    pushNodo("nombre", current->getToken());
    popNodo(nodoActual->getParent()); // volver a contexto
    advance();

    popNodo(padre);
}

void Parser::metadato()
{
    auto padre = nodoActual;
    pushNodo("metadato");

    if (!match("@"))
        error("@", "metadato", "simbolo");
    advance();

    if (!match("identificador"))
        error("identificador", "metadato");
    pushNodo("nombre", current->getToken());
    popNodo(nodoActual->getParent()); // volver a metadato
    advance();

    popNodo(padre);
}

void Parser::mostrar()
{
    auto padre = nodoActual;
    pushNodo("mostrar");

    if (!match("mostrar"))
        error("mostrar", "mostrar", "palabra reservada");
    advance();

    expresion();

    popNodo(padre);
}

void Parser::exportar()
{
    auto padre = nodoActual;
    pushNodo("exportar");

    if (!match("exportar"))
        error("exportar", "exportar", "palabra reservada");
    advance();

    if (!match("identificador"))
        error("identificador", "exportar");
    pushNodo("nombre", current->getToken());
    popNodo(nodoActual->getParent()); // volver a exportar
    advance();

    if (!match("=>"))
        error("=>", "exportar", "simbolo");
    advance();

    if (!match("literal_cadena"))
        error("cadena de caracteres", "exportar", "tipo de dato");
    pushNodo("destino", current->getToken());
    popNodo(nodoActual->getParent()); // volver a exportar
    advance();

    popNodo(padre);
}

void Parser::retorno()
{
    auto padre = nodoActual;
    pushNodo("retornar");

    if (!match("retornar"))
        error("retornar", "retorno", "palabra reservada");
    advance();

    expresion();

    popNodo(padre);
}
