#ifndef __STENTRY_H
#define __STENTRY_H

#include "token.h"

class STEntry
{
public:
    enum Types
    {
        NONE,
        INT,
        INTARRAY,
        BOOL,
        USERDEF
    };

    Token  token;
    bool   reserved;
    int    type;

    STEntry(Token tok, int type = NONE) : token{tok}, reserved{false}, type{type} {}
    STEntry(Token tok, bool res) : token{tok}, reserved{res}, type{NONE} {}
};

#endif
