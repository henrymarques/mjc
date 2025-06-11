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
    SymbolTable();
    SymbolTable(SymbolTable*);

    bool add(STEntry);
    STEntry* find(string);
};

#endif
