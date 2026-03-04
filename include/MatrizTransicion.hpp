#ifndef MATRIZTRANSICION_HPP
#define MATRIZTRANSICION_HPP

const int ESTADOS = 116;
const int SIMBOLOS = 28;

// clang-format off
enum class Simbolo {
    Delimitador = 0,
    A, B, C, D, E, F, H, I, L, M, N, O, P, R, S, T, U, Y,
    Cero,
    Digito,
    Punto,
    Exponente,
    Letra,
    GuionBajo,
    Menos,
    ComillasDobles,
    ComillasSimples,
    Error,
    Total
};

extern const int matriz_transicion[ESTADOS][SIMBOLOS];
#endif
