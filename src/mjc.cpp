#include "parser.h"

#include <fstream>

using std::cout;
std::ifstream fin;

void
scannerTest()
{
    auto globalST = new SymbolTable();
    auto scanner = new Scanner(globalST);

    Token *token;
    do
    {
        try
        {
            token = scanner->scan();
        }
        catch(Error err) {
            err.what();
            return;
        }
#ifdef _DEBUG
        std::cout << token->lexeme << ' ';
#endif
    } while (token->type != Types::END_OF_FILE);

    delete globalST;
    delete scanner;
}

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
        cout << "Não foi possível abrir o arquivo\n";
        return 1;
    }

    scannerTest();
    
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
