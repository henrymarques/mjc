#ifndef __SYMBOLTABLE_H
#define __SYMBOLTABLE_H

#include <unordered_map>

#include "stentry.h"

using std::string;

// Classe que representa uma tabela de símbolos. A classe utiliza
// a classe map para armazenar os símbolos. map implementa
// uma árvore que associa uma chave a um valor: em nosso casa a chave é uma
// string e o valor é um ponteiro para um objeto da classe STEntry.
class SymbolTable
{
private:
    SymbolTable* parent;   // Referência à tabela pai (escopo imediatamente anterior).
    std::unordered_map<string, STEntry> symbols;  // Armazena os símbolos do escopo corrente.

public:
    SymbolTable() : parent{nullptr}
    {
        using E = STEntry;

        add(E(Token(CLASS, "class"), true));
        add(E(Token(ELSE, "else"), true));
        add(E(Token(EXTENDS, "extends"), true));
        add(E(Token(FALSE, "false"), true));
        add(E(Token(IF, "if"), true));
        add(E(Token(INT, "int"), true));
        add(E(Token(BOOLEAN, "boolean"), true));
        add(E(Token(LENGTH, "length"), true));
        add(E(Token(MAIN, "main"), true));
        add(E(Token(NEW, "new"), true));
        add(E(Token(PUBLIC, "public"), true));
        add(E(Token(RETURN, "return"), true));
        add(E(Token(STATIC, "static"), true));
        add(E(Token(STRING, "String"), true));
        add(E(Token(PRINTLN, "System.out.println"), true));
        add(E(Token(THIS, "this"), true));
        add(E(Token(TRUE, "true"), true));
        add(E(Token(VOID, "void"), true));
        add(E(Token(WHILE, "while"), true));
    }

    SymbolTable(SymbolTable* p) : parent{p} {}

    bool add(STEntry);
    STEntry* find(string);
    STEntry* findScope(string);
};

#endif
