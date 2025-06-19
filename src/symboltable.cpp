#include "symboltable.h"

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
