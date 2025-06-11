#include "parser.h"

#include <sstream>

Parser::~Parser()
{
    delete scanner;
    delete globalST;
}

Parser::Parser()
{
    globalST = new SymbolTable();
    scanner = new Scanner(globalST);

    advance();
}

void
Parser::run()
{
    program();

    matchOrError(END_OF_FILE);

    std::cout << "Compilação encerrada com sucesso!\n";
}

void
Parser::program()
{
    if (lToken->type == CLASS) classList();
}

void
Parser::classList()
{
    do { classDecl(); } while (lToken->type == CLASS);
}

void
Parser::classDecl()
{
    match(CLASS);
    match(ID);

    if (lToken->type == EXTENDS)
    {
        advance();
        match(ID);
    }
}
