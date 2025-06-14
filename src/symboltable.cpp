#include "symboltable.h"

// Construtor que instancia uma nova tabela sem especificar uma tabela pai.
SymbolTable::SymbolTable() : parent{ nullptr }
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

// Construtor que instancia uma nova tabela especificando uma tabela pai.
SymbolTable::SymbolTable(SymbolTable* p) : parent{p} {}

// Tenta adicionar um novo símbolo à tabela corrente. Primeiramente, verifica
// se já existe uma entrada para o lexema na tabela corrente e, caso haja,
// a inserção não é realizada e o método retorna false; caso contrário, a
// inserção é efetivada e true é retornado.
bool
SymbolTable::add(STEntry entry)
{
    const auto& [pos, success] = symbols.insert({entry.token.lexeme, entry});
    return success;
}

STEntry*
SymbolTable::find(string lexeme)
{
    for (SymbolTable* st = this; st != nullptr; st = st->parent)
    {
        auto found = st->symbols.find(lexeme);
        if (found != st->symbols.end())
            return &found->second;
    }
    return nullptr;
}
