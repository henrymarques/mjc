#ifndef __PARSER_H
#define __PARSER_H

#include "scanner.h"
#include "error.h"

#include <algorithm>

extern std::unordered_map<int, std::string> tokenTypeMap;

class Parser
{
private:
    Scanner* scanner;
    Token* lToken;
    SymbolTable* globalST;
    stringstream error;

    void advance();
    void match(int);
    bool recTipos();

    std::vector<Types> tokens{
        RCBRAC, // }
        SEMI, // ;
        END_OF_FILE
    };

public:
    Parser();
    ~Parser();
    void run();
    bool nextIs(int);
    bool nextIs(const string&);
    void program();
    void mainClass();
    void classDeclaration();
    void varDeclaration();
    void methodDeclaration();
    void type();
    void statement();
    void expression();
    void expressionLinha();
    void panic();
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
    }
    else {
        stringstream error;
        error << "Esperado '" << tokenTypeMap[type] << "' na linha " << scanner->getLine() << " obtido " << *lToken << '\n';
        std::cout << error.str();

        this->error << error.str();
        panic();
    }
}

inline void Parser::panic()
{
    // type tá na lista
    while (std::find(tokens.begin(), tokens.end(), lToken->type) == tokens.end())
    {
        advance();
    }
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