#ifndef __SCANNER_H
#define __SCANNER_H

#include "token.h"
#include "symboltable.h"

#include <iostream>
#include <sstream>

using std::string;
using std::stringstream;

class Scanner
{
private:
    string       input;
    char         peek;
    int          pos = 0;
    int          line = 1;

    SymbolTable* st;
    Token token;

public:
    Scanner(SymbolTable*);
    Token* scan();

    // int getLine() const { return line; }
};

#endif
