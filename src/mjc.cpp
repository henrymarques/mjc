#include "parser.h"

#include <fstream>
#include <filesystem>

using std::cout;
std::ifstream fin;

void fullTest()
{
    for (auto const& dir_entry : std::filesystem::directory_iterator{ "tests" })
    {
        if (dir_entry.is_directory()) continue;
        fin.open(dir_entry.path());

        try
        {
            Parser parser;
            parser.run();
        }
        catch (Error err)
        {
            err.what();
        }

        fin.close();
        break;
    }
    exit(0);
}

int
main(int argc, char *argv[])
{
    std::locale::global(std::locale(".utf8"));

    fullTest();

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
    
    try
    {
        Parser parser;
        parser.run();
    }
    catch (Error err)
    {
        err.what();
    }

    return 0;
}
