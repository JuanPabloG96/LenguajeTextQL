#include "Data/LexerData.hpp"

namespace lexer {
// clang-format off
const std::unordered_map<int, std::string> LexicalErrorMessages = {
    {-1, "error de creacion de numero entero"},
    {-2, "error de creacion de numero real"},
    {-3, "error de creacion de numero notacion cientifica"},
    {-4, "error de creacion de identificador"},
    {-5, "error de creacion de caracter"},
    {-6, "error de creacion de cadena"},
    {-7, "error desconocido"}};

const std::unordered_map<int, std::string> TokenMap = {
    {1000, "numero entero"},
    {2000, "numero real"},
    {3000, "numero notacion_cientifica"},
    {4000, "identificador"},
    {5000, "caracter"},
    {6000, "cadena de caracteres"},
    {7001, "booleano"},
    {7002, "cadena"},
    {7003, "caracter"},
    {7004, "caso"},
    {7005, "entero"},
    {7006, "escribir"},
    {7007, "fin"},
    {7008, "funcion"},
    {7009, "hacer"},
    {7010, "inicio"},
    {7011, "leer"},
    {7012, "mientras"},
    {7013, "o"},
    {7014, "para"},
    {7015, "procedimiento"},
    {7016, "real"},
    {7017, "retornar"},
    {7018, "si"},
    {7019, "sino"},
    {7020, "y"}};
}
// clang-format on
