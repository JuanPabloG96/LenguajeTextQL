#include "Lexer.hpp"
#include <fstream>
#include <iostream>

int main()
{

    std::ifstream archivo("../docs/test3.txt");
    if (!archivo.is_open())
    {
        std::cerr << "Error: no se pudo abrir el archivo\n";
        return -1;
    }

    char c;
    Lexer lexer;

    while (archivo.get(c))
    {
        lexer.createTokenList(c);
    }
    lexer.lexerFinish();

    lexer.printTokenList();
    std::cout << std::endl;
    lexer.printErrors();

    archivo.close();
    return 0;
}
