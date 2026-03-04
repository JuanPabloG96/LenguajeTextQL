#ifndef TRANSITIONMATRIX_HPP
#define TRANSITIONMATRIX_HPP

namespace lexer {
const int ESTADOS = 116;
const int SIMBOLOS = 28;

// clang-format off
enum class Symbol {
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
    Total
};

extern const int transitionMatrix[ESTADOS][SIMBOLOS];
}
#endif
