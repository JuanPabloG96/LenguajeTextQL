#pragma once

#include "NodoAST.hpp"

#include <fstream>
#include <iostream>
#include <memory>
#include <string>

class ExportadorDot
{
private:
    int contadorId;

    std::string getColor(const std::string& tipo)
    {
        if (tipo == "statements")
            return "lightblue";
        if (tipo == "defFuncion" || tipo == "defEsquema" ||
            tipo == "asignacionFuente")
            return "lightgreen";
        if (tipo == "declaracionVar" || tipo == "reasignacion")
            return "lightyellow";
        if (tipo == "escanear" || tipo == "transformar" || tipo == "pipeline" ||
            tipo == "listaContextos")
            return "lightsalmon";
        if (tipo == "expresion" || tipo == "op")
            return "lavender";
        if (tipo == "condicional" || tipo == "sino_si" || tipo == "sino")
            return "lightcoral";
        if (tipo == "mostrar" || tipo == "exportar" || tipo == "retornar")
            return "lightcyan";
        if (tipo == "campos" || tipo == "campo" || tipo == "parametros")
            return "wheat";
        return "white";
    }

    bool esTerminal(std::shared_ptr<NodoAST> nodo)
    {
        return nodo->getHijos().empty() && !nodo->getValor().empty();
    }

    void exportarNodo(std::shared_ptr<NodoAST> nodo, std::ofstream& archivo)
    {
        int idActual = contadorId++;

        if (esTerminal(nodo))
        {
            // Nodo terminal: mostrar solo el valor (el token), con forma de
            // elipse
            archivo << "  n" << idActual << " [label=\"" << nodo->getValor()
                    << "\""
                    << ", shape=ellipse"
                    << ", style=filled"
                    << ", fillcolor=\"lightyellow\""
                    << "];\n";
        }
        else
        {
            // Nodo interno: mostrar tipo con color segun categoria
            std::string etiqueta = nodo->getTipo();
            std::string color = getColor(nodo->getTipo());

            archivo << "  n" << idActual << " [label=\"" << etiqueta << "\""
                    << ", style=filled"
                    << ", fillcolor=\"" << color << "\""
                    << "];\n";

            for (auto& hijo : nodo->getHijos())
            {
                int idHijo = contadorId;
                exportarNodo(hijo, archivo);
                archivo << "  n" << idActual << " -> n" << idHijo << ";\n";
            }
        }
    }

public:
    ExportadorDot() : contadorId(0)
    {
    }

    void exportar(std::shared_ptr<NodoAST> raiz,
                  const std::string& nombreArchivo)
    {
        contadorId = 0;
        std::ofstream archivo(nombreArchivo);

        if (!archivo.is_open())
        {
            std::cerr << "Error: no se pudo crear el archivo " << nombreArchivo
                      << "\n";
            return;
        }

        archivo << "digraph AST {\n";
        archivo << "  node [shape=box, fontname=\"monospace\"];\n";
        archivo << "  rankdir=TB;\n\n";

        exportarNodo(raiz, archivo);

        archivo << "}\n";
        archivo.close();
    }
};
