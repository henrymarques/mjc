#ifndef __TOKEN_H
#define __TOKEN_H

#include <string>
#include <iostream>
#include <unordered_map>

using string = std::string;

enum Types
{
    UNDEF,
    ID,
    INTEGER_LITERAL,
    OP,
    SEP,
    BOOLEAN,
    CLASS,
    ELSE,
    EXTENDS,
    FALSE,
    IF,
    INT,
    LENGTH,
    MAIN,
    NEW,
    PUBLIC,
    RETURN,
    STATIC,
    STRING,
    PRINTLN,
    THIS,
    TRUE,
    VOID,
    WHILE,
    END_OF_FILE
};

class Token
{
public:
    int    type;
    string lexeme;

    Token()
    {
        this->type = UNDEF;
    }

    Token(int type)
    {
        this->type = type;
    }

    Token(int type, string l)
    {
        this->type = type;
        lexeme     = l;
    }

    // std::ostream& operator<<(std::ostream&, const Token&);
};

inline std::ostream&
operator<<(std::ostream& os, const Token& token)
{
    static std::unordered_map<int, std::string> tokenTypeMap = {
        { UNDEF, "UNDEF" },
        { ID, "ID" },
        { INTEGER_LITERAL, "INTEGER_LITERAL" },
        { OP, "OPERATOR" },
        { SEP, "SEPARATOR" },
        { BOOLEAN, "BOOLEAN" },
        { CLASS, "CLASS" },
        { ELSE, "ELSE" },
        { EXTENDS, "EXTENDS" },
        { FALSE, "FALSE" },
        { IF, "IF" },
        { INT, "INT" },
        { LENGTH, "LENGTH" },
        { MAIN, "MAIN" },
        { NEW, "NEW" },
        { PUBLIC, "PUBLIC" },
        { RETURN, "RETURN" },
        { STATIC, "STATIC" },
        { STRING, "STRING" },
        { PRINTLN, "PRINTLN" },
        { THIS, "THIS" },
        { TRUE, "TRUE" },
        { VOID, "VOID" },
        { WHILE, "WHILE" },
        { END_OF_FILE, "END_OF_FILE" }
    };

    return os << tokenTypeMap[token.type];
}

#endif
