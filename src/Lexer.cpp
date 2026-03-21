#include "Lexer.hpp"
#include "Data/LexerData.hpp"
#include "Data/TransitionMatrix.hpp"
#include <cctype>
#include <iomanip>
#include <iostream>

Lexer::Lexer() : state(0), category(0), line(1), column(0)
{
}

void Lexer::createTokenList(char c)
{
    auto symbol = classifyChar(c);

    if (state == 0 && symbol == Symbol::Delimitador)
        return;

    category = static_cast<int>(symbol);
    state = transitionMatrix[state][category];

    std::cout << "Columna: " << category;
    std::cout << " Estado = " << state << ", ";

    // Si el estado es un error
    if (state < 0)
    {
        auto error = LexicalErrorMessages.find(state);

        if (c != '\n' && c != '\t')
        {
            buffer += c;
        }

        if (error != LexicalErrorMessages.end())
        {
            errors.push_back({line, column, error->second, buffer});
        }

        state = 0;
        buffer.clear();
    }
    // Si el estado es de aceptación
    else if (state >= 1000)
    {
        auto lexeme = buffer;
        if (state == 3000 && PalabrasReservadas.count(lexeme))
        {
            auto token = PalabrasReservadas.find(buffer);
            TokenList.add(*token, lexeme);
        }
        else
        {
            auto token = TokenMap.find(state);
            if (token != TokenMap.end())
            {
                TokenList.add(token->second, lexeme);
            }
        }

        state = 0;
        buffer.clear();
        // Repetir el proceso para el siguiente caracter para evitar perder el token de simbolos
        createTokenList(c);
    }
    else
    {
        buffer += c;
    }

    if (c == '\n')
    {
        line++;
        column = 0;
    }
    else
        column++;
}

void Lexer::lexerFinish()
{
    if (state == 12)
    {
        auto error = "Error: no se a cerrado la cadena al final del archivo";
        buffer = buffer.substr(0, buffer.size() - 1);
        errors.push_back({line, column, error, buffer});
    }
}

Symbol Lexer::classifyChar(char c)
{
    switch (c)
    {
    case '.':
        return Symbol::Punto;
        break;
    case '_':
        return Symbol::GuionBajo;
        break;
    case '+':
        return Symbol::Mas;
        break;
    case '-':
        return Symbol::Menos;
        break;
    case '"':
        return Symbol::ComillaDoble;
        break;
    case '\'':
        return Symbol::ComillaSimple;
        break;
    case '>':
        return Symbol::Mayor;
        break;
    case ':':
        return Symbol::DosPuntos;
        break;
    case '~':
        return Symbol::Virgulilla;
        break;
    case '=':
        return Symbol::Igual;
        break;
    case '$':
        return Symbol::Dolar;
        break;
    case '@':
        return Symbol::Arroba;
        break;
    case '#':
        return Symbol::Hash;
        break;
    case '?':
        return Symbol::Interrogacion;
        break;
    case '&':
        return Symbol::Ampersand;
        break;
    case '|':
        return Symbol::BarraVertical;
        break;
    case '(':
        return Symbol::AbreParentesis;
        break;
    case ')':
        return Symbol::CierraParentesis;
        break;
    case '[':
        return Symbol::AbreCorchete;
        break;
    case ']':
        return Symbol::CierraCorchete;
        break;
    case '{':
        return Symbol::AbreLlave;
        break;
    case '}':
        return Symbol::CierraLlave;
        break;
    case ',':
        return Symbol::Coma;
        break;
    }

    if (c == ' ' || c == '\n' || c == '\t')
        return Symbol::Delimitador;

    if (std::isdigit((unsigned char)c))
    {
        if (c == '0')
            return Symbol::Cero;
        return Symbol::Digito;
    }

    if (std::isalpha((unsigned char)c))
        return Symbol::Letra;

    return Symbol::Indefinido;
}

void Lexer::printTokenList()
{
    auto current = TokenList.getHead();

    std::cout << std::endl;
    std::cout << std::left << std::setw(5) << "\n#" << std::setw(50) << "Tipo"
              << "Token" << std::endl;

    std::cout << std::string(80, '-') << std::endl;

    while (current)
    {
        std::cout << std::left << std::setw(5) << current->getId() << std::setw(50)
                  << current->getTokenType() << current->getToken() << std::endl;

        current = current->getNext();
    }
}
void Lexer::printErrors()
{
    for (auto error : errors)
    {
        std::cout << "Error en la linea " << error.line << ", columna "
                  << error.column << ": " << error.message << " (" << error.lexeme
                  << ")." << std::endl;
    }
}
