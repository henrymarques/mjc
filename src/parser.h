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
    bool match(int);
    void matchOrError(int);

    void program();
    void classList();
    void classDecl();

public:
    Parser();
    ~Parser();
    void run();
};

inline void
Parser::advance()
{
    lToken = scanner->scan();
}

inline bool
Parser::match(int type)
{
    if (lToken->type == type)
    {
        advance();
        return true;
    }

    return false;
}

inline void
Parser::matchOrError(int type)
{
    if (!match(type))
    {
        std::stringstream ss;
        ss << "";

        throw SyntaxError(ss.str());
    }
}

#endif