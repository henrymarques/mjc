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
    std::cout << "Compilação encerrada com sucesso!\n";
}

bool 
Parser::nextIs(int type)
{
    return lToken->type == type;
}

bool Parser::nextIs(const string& lexeme)
{
    return lToken->lexeme == lexeme;
}

void Parser::program()
{
    mainClass();
    while (nextIs(CLASS)) {
        classDeclaration();
    }
    match(END_OF_FILE);
}

void Parser::mainClass()
{
    match(CLASS);
    match(ID);
    match(SEP);
    match(PUBLIC);
    match(STATIC);
    match(VOID);
    match(MAIN);
    match(SEP); 
    match(STRING);
    match(SEP); 
    match(SEP); 
    match(ID);
    match(SEP); 
    match(SEP); 
    statement();
    match(SEP); 
    match(SEP); 
}

void Parser::classDeclaration()
{
    match(CLASS);
    match(ID);
    if (nextIs(EXTENDS)) {
        match(EXTENDS);
        match(ID);
    }
    match(SEP);
    while (!nextIs("}")) {
        if (nextIs(PUBLIC)) {
            methodDeclaration();
        }
        else {
            varDeclaration();
        }
    }
    match(SEP);
}

void Parser::varDeclaration()
{
    type();
    match(ID);
    match(SEP);
}

void Parser::methodDeclaration()
{
    match(PUBLIC);
    type();
    match(ID);
    match(SEP); 
    if (nextIs(INT) || nextIs(BOOLEAN) || nextIs(ID)) {
        type();
        match(ID);
        while (nextIs(",")) {
            match(SEP); 
            type();
            match(ID);
        }
    }
    match(SEP); 
    match(SEP); 
    while (nextIs(INT) || nextIs(BOOLEAN) ||
        (nextIs(ID) && lToken->lexeme != "return")) {
        varDeclaration();
    }
    while (!nextIs(RETURN)) {
        statement();
    }
    match(RETURN);
    expression();
    match(SEP); 
    match(SEP); 
}

void Parser::type()
{
    if (nextIs(INT)) {
        match(INT);
        if (nextIs("[")) {
            match(SEP); 
            match(SEP); 
        }
    }
    else if (nextIs(BOOLEAN)) {
        match(BOOLEAN);
    }
    else {
        match(ID);
    }
}

void Parser::statement()
{
    if (nextIs("{")) {
        match(SEP); 
        while (!nextIs("}")) {
            statement();
        }
        match(SEP); 
    }
    else if (nextIs(IF)) {
        match(IF);
        match(SEP); 
        expression();
        match(SEP); 
        statement();
        match(ELSE);
        statement();
    }
    else if (nextIs(WHILE)) {
        match(WHILE);
        match(SEP); 
        expression();
        match(SEP); 
        statement();
    }
    else if (nextIs(PRINTLN)) {
        match(PRINTLN);
        match(SEP); 
        expression();
        match(SEP); 
        match(SEP); 
    }
    else {
        match(ID);
        if (nextIs("[")) {
            match(SEP); 
            expression();
            match(SEP); 
        }
        match(OP); 
        expression();
        match(SEP); 
    }
}

void Parser::expression()
{
    if (nextIs(INTEGER_LITERAL)) {
        match(INTEGER_LITERAL);
    }
    else if (nextIs(TRUE)) {
        match(TRUE);
    }
    else if (nextIs(FALSE)) {
        match(FALSE);
    }
    else if (nextIs(ID)) {
        match(ID);
    }
    else if (nextIs(THIS)) {
        match(THIS);
    }
    else if (nextIs(NEW)) {
        match(NEW);
        if (nextIs(INT)) {
            match(INT);
            match(SEP); 
            expression();
            match(SEP); 
        }
        else {
            match(ID);
            match(SEP); 
            match(SEP); 
        }
    }
    else if (nextIs("!")) {
        match(OP); 
        expression();
    }
    else if (nextIs("(")) {
        match(SEP); 
        expression();
        match(SEP); 
    }
    expressionLinha();
}

void Parser::expressionLinha()
{
    if (nextIs(OP) && lToken->lexeme != "=") {
        match(OP);
        expression();
    }
    else if (nextIs("[")) {
        match(SEP); 
        expression();
        match(SEP); 
        expressionLinha();
    }
    else if (nextIs(".")) {
        match(SEP); 
        if (nextIs(LENGTH)) {
            match(LENGTH);
        }
        else {
            match(ID);
            match(SEP); 
            if (!nextIs(")")) {
                expression();
                while (nextIs(",")) {
                    match(SEP); 
                    expression();
                }
            }
            match(SEP); 
        }
        expressionLinha();
    }
}

