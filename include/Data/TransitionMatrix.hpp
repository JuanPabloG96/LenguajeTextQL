#ifndef TRANSITIONMATRIX_HPP
#define TRANSITIONMATRIX_HPP

namespace lexer
{
const int ESTADOS = 116;
const int SIMBOLOS = 29;

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
    MasMenos,
    ComillasDobles,
    ComillasSimples,
    Error,
    Total
};

extern const int transitionMatrix[ESTADOS][SIMBOLOS];
}
#endif
