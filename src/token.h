#ifndef __TOKEN_H
#define __TOKEN_H

#include <string>
#include <iostream>
#include <unordered_map>

using string = std::string;

enum Tag
{
    UNDEF,
    ID,
    INTEGER_LITERAL,
    /*
    LT,
    GT,
    PLUS,
    MINUS,
    MULT,
    DIV,
    EQ,
    NEQ,
    AND,
    */
    OP,
    ATTRIB,
    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,
    LCBRAC,
    RCBRAC,
    SEMI,
    COMMA,
    DOT,
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
    COMMENT,
    END_OF_FILE
};

static std::unordered_map<int, std::string> tokenTypeMap{
        { UNDEF, "UNDEF" },
        { ID, "ID" },
        { INTEGER_LITERAL, "INTEGER_LITERAL" },
        /*
        { LT, "OPERATOR" },
        { GT, "OPERATOR" },
        { PLUS, "OPERATOR" },
        { MINUS, "OPERATOR" },
        { MULT, "OPERATOR" },
        { DIV, "OPERATOR" },
        { EQ, "OPERATOR" },
        { NEQ, "OPERATOR" },
        { AND, "OPERATOR" },
        */
        { OP, "OPERATOR" },
        { ATTRIB, "=" },
        { LPAREN, "(" },
        { RPAREN, ")" },
        { LBRACE, "[" },
        { RBRACE, "]" },
        { LCBRAC, "{" },
        { RCBRAC, "}" },
        { SEMI, ";" },
        { COMMA, "," },
        { DOT, "." },
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
        lexeme = l;
    }
};

inline std::ostream&
operator<<(std::ostream& os, const Token& token)
{
    return os << tokenTypeMap[token.type];
}

#endif
