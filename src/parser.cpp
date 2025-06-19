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
    lToken = scanner->scan();
}

void
Parser::run()
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

inline bool
Parser::nextIs(int type)
{
    return lToken->type == type;
}

bool Parser::nextIs(const string& lexeme)
{
    return lToken->lexeme == lexeme;
}

bool Parser::recTipos()
{
    if (nextIs(INT) || nextIs(BOOLEAN))
        return true;

    if (nextIs(ID))
    {
        STEntry* e = globalST->find(lToken->lexeme);
        return e != nullptr && e->reserved == false;
    }
    return false;
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
    
    /*if (globalST->find(lToken->lexeme) != nullptr)
    {
        stringstream erro;
        erro << "Classe " << lToken->lexeme << " já declarada\n";
        throw Error(erro.str());
    }
    globalST->add(STEntry(Token(ID, lToken->lexeme)));*/

    match(ID);

    if (nextIs(EXTENDS))
    {
        match(EXTENDS);
        match(ID);
    }
    match(LCBRAC);
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
}

void Parser::varDeclaration()
{
    type();
    match(ID);
    match(SEMI);
}

void Parser::methodDeclaration()
{
    match(PUBLIC);
    type();
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
}

void Parser::type()
{
    if (nextIs(INT))
    {
        match(INT);
        if (nextIs(LBRACE))
        {
            match(LBRACE);
            match(RBRACE);
        }
    }
    else if (nextIs(BOOLEAN))
    {
        match(BOOLEAN);
    }
    else
    {
        match(ID);
    }
}

void Parser::statement()
{
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
       /*
        if (!nextIs(ID))
        {
            stringstream error;
            error << "not allowed: " << lToken->type << ' ' << lToken->lexeme << '\n';
            std::cout << error.str();
            this->error << error.str();

            panic();
            return;
        }
       */

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
    else if (nextIs("!"))
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