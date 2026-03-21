#include "Data/LexerData.hpp"

// clang-format off
const std::unordered_map<int, std::string> LexicalErrorMessages = {
    {-1, "Error: numero entero mal formado"},
    {-2, "Error: numero flotante mal formado"},
    {-3, "Error: identificador invalido (no puede empezar con numero o simbolo)"},
    {-4, "Error: caracter mal formado o sin cerrar"},
    {-5, "Error: cadena de caracteres mal formada"},
    {-6, "Error: simbolo compuesto incompleto"},
    {-7, "Error: simbolo no reconocido por el lenguaje"}
}; // clang-format on

const std::unordered_map<int, std::string> TokenMap = {
    // Tipos de datos (valores)
    {1000, "numero entero"},
    {2000, "numero real"},
    {3000, "identificador"},
    {4000, "literal caracter"},
    {5000, "literal cadena"},
    // Simbolos (separadores)
    {6001, "abre_parentesis"},   // (
    {6002, "cierra_parentesis"}, // )
    {6003, "abre_llave"},        // {
    {6004, "cierra_llave"},      // }
    {6005, "abre_corchete"},     // [
    {6006, "cierra_corchete"},   // ]
    {6007, "coma"},              // ,
    // Simbolos reservados
    {7029, "#"},
    {7030, "$"},
    {7031, "..."},
    {7032, "?"},
    {7033, "@"},
    {7034, "->"},
    {7035, "::"},
    {7036, "=>"},
    {7037, "~~"},
    {7038, "+"},
    {7040, "-"},
    {7041, "="},
    {7042, "=="},
    {7043, "&&"},
    {7044, "||"}};

const std::unordered_set<std::string> PalabrasReservadas = {
    "abrir", "agrupar", "booleano", "buscar", "caracter", "const", "donde", "entero", "entonces", "entonces si", "escanear", "esquema", "exportar", "extraer", "flotante", "frecuencia", "fuente", "funcion", "limite", "lista", "mostrar", "normalizar", "ordenar", "puntuacion", "retornar", "si", "sino", "texto"};
