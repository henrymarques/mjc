#include "parser.h"

#include <sstream>

Parser::~Parser()
{
    delete scanner;
    delete symTable;
}

Parser::Parser()
{
    symTable = new SymbolTable();
    scanner = new Scanner(symTable);
    lToken = scanner->scan();
}

void Parser::run()
{
    program();
    advance();
    auto err = error.str();
    if (err == "")
    {
        std::cout << "Compilação encerrada com sucesso!\n";
    }
    else
    {
        std::cout << "Compilação encerrada erros.\n";
    }
}

void Parser::program()
{
    mainClass();
    while (nextIs(CLASS))
    {
        classDeclaration();
    }
    match(END_OF_FILE);
}

void Parser::mainClass()
{
    match(CLASS);
    match(ID);
    match(LCBRAC);
    match(PUBLIC);
    match(STATIC);
    match(VOID);
    match(MAIN);
    match(LPAREN);
    match(STRING);
    match(LBRACE);
    match(RBRACE);
    match(ID);
    match(RPAREN);
    match(LCBRAC);
    statement();
    match(RCBRAC);
    match(RCBRAC);
}

void Parser::classDeclaration()
{
    match(CLASS);

    match(ID);

    if (nextIs(EXTENDS))
    {
        match(EXTENDS);
        match(ID);
    }
    match(LCBRAC);

    SymbolTable* saved = symTable;
    symTable = new SymbolTable(symTable);

    while (!nextIs(RCBRAC))
    {
        if (nextIs(PUBLIC))
        {
            methodDeclaration();
        }
        else
        {
            varDeclaration();
        }
    }
    match(RCBRAC);

    delete symTable;
    symTable = saved;
}

int Parser::type()
{
    int varType;
    if (nextIs(INT))
    {
        match(INT);
        if (nextIs(LBRACE))
        {
            match(LBRACE);
            match(RBRACE);
            varType = STEntry::Types::INTARRAY;
        }

        varType = STEntry::Types::INT;
    }
    else if (nextIs(BOOLEAN))
    {
        match(BOOLEAN);
        varType = STEntry::Types::BOOL;
    }
    else
    {
        auto symbol = symTable->find(lToken->lexeme);
        if (symbol == nullptr)
        {
            varType = STEntry::Types::NONE;
            stringstream error;
            error << "Símbolo " << lToken->lexeme << " não definido na linha " << scanner->getLine() << '\n';
            this->error << error.str();
            std::cout << error.str();
        }
        else
        {
            varType = STEntry::Types::USERDEF;
        }
        match(ID);
    }

    return varType;
}

void Parser::varDeclaration()
{
    int varType = type();

    STEntry symbol{*lToken, varType};
    symTable->add(symbol);

    match(ID);
    match(SEMI);
}

void Parser::methodDeclaration()
{
    match(PUBLIC);
    int methodType = type();

    STEntry symbol{*lToken, methodType};
    symTable->add(symbol);

    match(ID);
    match(LPAREN);
    if (nextIs(INT) || nextIs(BOOLEAN) || nextIs(ID))
    {
        type();
        match(ID);
        while (nextIs(COMMA))
        {
            match(COMMA);
            type();
            match(ID);
        }
    }
    match(RPAREN);
    match(LCBRAC);

    SymbolTable* saved = symTable;
    symTable = new SymbolTable(symTable);

    while ( recTipos() )
    {
        varDeclaration();
    }
    while (!nextIs(RETURN))
    {
        if (nextIs(RCBRAC))
        {
            break;
        }
        statement();
    }
    match(RETURN);
    expression();
    match(SEMI);
    match(RCBRAC);

    delete symTable;
    symTable = saved;
}

void Parser::statement()
{
    SymbolTable* saved = symTable;
    symTable = new SymbolTable(symTable);
    
    if (nextIs(LCBRAC))
    {
        match(LCBRAC);
        while (!nextIs(RCBRAC))
        {
            statement();
        }
        match(RCBRAC);
    }
    else if (nextIs(IF))
    {
        match(IF);
        match(LPAREN);
        expression();
        match(RPAREN);
        statement();
        match(ELSE);
        statement();
    }
    else if (nextIs(WHILE))
    {
        match(WHILE);
        match(LPAREN);
        expression();
        match(RPAREN);
        statement();
    }
    else if (nextIs(PRINTLN))
    {
        match(PRINTLN);
        match(LPAREN);
        expression();
        match(RPAREN);
        match(SEMI);
    }
    else
    {
        match(ID);
        if (nextIs(LBRACE))
        {
            match(LBRACE);
            expression();
            match(RBRACE);
        }
        match(ATTRIB);
        expression();
        match(SEMI);
    }

    delete symTable;
    symTable = saved;
}

void Parser::expression()
{
    if (nextIs(INTEGER_LITERAL))
    {
        match(INTEGER_LITERAL);
    }
    else if (nextIs(TRUE))
    {
        match(TRUE);
    }
    else if (nextIs(FALSE))
    {
        match(FALSE);
    }
    else if (nextIs(ID))
    {
        match(ID);
    }
    else if (nextIs(THIS))
    {
        match(THIS);
    }
    else if (nextIs(NEW))
    {
        match(NEW);
        if (nextIs(INT))
        {
            match(INT);
            match(LBRACE);
            expression();
            match(RBRACE);
        }
        else
        {
            match(ID);
            match(LPAREN);
            match(RPAREN);
        }
    }
    else if (nextIs(OP) && lToken->lexeme != "!")
    {
        match(OP);
        expression();
    }
    else if (nextIs(LPAREN))
    {
        match(LPAREN);
        expression();
        match(RPAREN);
    }
    expressionLinha();
}

void Parser::expressionLinha()
{
    if (nextIs(OP) && lToken->lexeme != "=")
    {
        match(OP);
        expression();
    }
    else if (nextIs(LBRACE))
    {
        match(LBRACE);
        expression();
        match(RBRACE);
        expressionLinha();
    }
    else if (nextIs(DOT))
    {
        match(DOT);
        if (nextIs(LENGTH))
        {
            match(LENGTH);
        }
        else
        {
            match(ID);
            if (nextIs(LPAREN))
            {
                match(LPAREN);
                if (!nextIs(RPAREN))
                {
                    expression();
                    while (nextIs(COMMA))
                    {
                        match(COMMA);
                        expression();
                    }
                }
                match(RPAREN);
            }
        }
        expressionLinha();
    }
}