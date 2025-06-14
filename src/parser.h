#ifndef __PARSER_H
#define __PARSER_H

#include "scanner.h"
#include "error.h"

class Parser
{
private:
    Scanner* scanner;
    Token* lToken;
    SymbolTable* globalST;
    void advance();
    void match(int);
    void matchOrError(int type);

public:
    Parser();
    ~Parser();
    void run();
    bool nextIs(int t);
    bool nextIs(const string& lexeme);
    void program();
    void mainClass();
    void classDeclaration();
    void varDeclaration();
    void methodDeclaration();
    void params();
    void type();
    void statement();
    void expression();
    void expressionLinha();
    bool op();
    void expressionsList();
};

inline void
Parser::advance()
{
    do {
        lToken = scanner->scan();
    } while (lToken->type == COMMENT);
}

inline void
Parser::match(int type)
{
    if (lToken->type == type)
    {
        advance();
        //return true;
    }
    else {
        stringstream err;
        err << "o que eh? '" << lToken->lexeme << "' na linha " << scanner->getLine();
        throw SyntaxError(err.str());
    }

    //return false;
}

/*
inline void
Parser::matchOrError(int type)
{
    if (!match(type))
    {
        std::stringstream ss;
        ss << "o que eh? '" << lToken->lexeme << "' na linha " << scanner->getLine();

        throw SyntaxError(ss.str());
    }
}*/

#endif 