#pragma once

const int ESTADOS = 40;
const int SIMBOLOS = 28;

// clang-format off
enum class Symbol {
    Delimitador = 0,
    Cero,
    Digito,
    Letra,
    Menos,
    GuionBajo,
    ComillaSimple,
    ComillaDoble,
    Mayor,
    DosPuntos,
    Virgulilla,
    Igual,
    Dolar,
    Arroba,
    Hash,
    Punto,
    Interrogacion,
    Mas,
    Ampersand,
    BarraVertical,
    AbreParentesis,
    CierraParentesis,
    AbreLlave,
    CierraLlave,
    AbreCorchete,
    CierraCorchete,
    Coma,
    Indefinido,
    Total
};
extern const int transitionMatrix[ESTADOS][SIMBOLOS];
