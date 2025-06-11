#ifndef __STENTRY_H
#define __STENTRY_H

#include "token.h"

class STEntry
{
public:
    Token  token;
    bool   reserved;

    STEntry(Token);
    STEntry(Token, bool);
};

#endif
