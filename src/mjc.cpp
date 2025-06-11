#include "parser.h"

#include <fstream>

using std::cout;
std::ifstream fin;

int
main(int argc, char *argv[])
{
    // Verifica se foi executado corretamente
    // Essa main espera receber o nome do arquivo a ser
    // executado na linha de comando.
    if (argc != 2)
    {
        cout << "Uso: ./mjc nome_arquivo.mj\n";
        return 1;
    }

    fin.open(argv[1]);
    if (!fin.is_open())
    {
        cout << "N�o foi poss�vel abrir o arquivo\n";
        return 1;
    }

    /*
    try
    {
        Parser parser;
        parser.run();
    }
    catch (Error err)
    {
        err.what();
    }
    */

    return 0;
}
