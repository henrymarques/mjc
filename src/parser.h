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
    SymbolTable* symTable;
    stringstream error;

    void advance();
    void match(int);
    bool recTipos();

    std::vector<Tag> tokens{
        RCBRAC, // }
        SEMI, // ;
        END_OF_FILE
    };

public:
    Parser();
    ~Parser();
    void run();
    void program();
    void mainClass();
    void classDeclaration();
    void varDeclaration();
    void methodDeclaration();
    STEntry::Types type();
    void statement();
    STEntry::Types expression();
    void expressionLinha(STEntry::Types);
    void panic();

    bool nextIs(int type)
    {
        return lToken->type == type;
    }
};

inline bool
Parser::recTipos()
{
    if (nextIs(INT) || nextIs(BOOLEAN))
        return true;

    if (nextIs(ID))
    {
        STEntry* e = symTable->find(lToken->lexeme);
        return e != nullptr && e->reserved == false && e->type == STEntry::Types::USERDEF;
    }

    return false;
}

inline void
Parser::advance()
{
    do
    {
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
    else
    {
        stringstream error;
        error << "Esperado '" << tokenTypeMap[type] << "' na linha " << scanner->getLine() << " obtido " << *lToken << '\n';
        std::cout << error.str();
        this->error << error.str();

        panic();
    }
}

inline void
Parser::panic()
{
    // type tá na lista
    while (std::find(tokens.begin(), tokens.end(), lToken->type) == tokens.end())
    {
        advance();
    }
}
#endif 