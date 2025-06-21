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
        USERDEF,
        POINTER
    };

    Token             token;
    bool              reserved;
    STEntry::Types    type;

    STEntry(Token tok, STEntry::Types type = NONE) : token{tok}, reserved{false}, type{type} {}
    STEntry(Token tok, bool res) : token{tok}, reserved{res}, type{NONE} {}
};

inline std::ostream& operator<<(std::ostream& os, STEntry::Types type)
{
    switch (type)
    {
    case 0:
        os << "NONE";
        break;
    case 1:
        os << "INT";
        break;
    case 2:
        os << "INTARRAY";
        break;
    case 3:
        os << "BOOL";
        break;
    case 4:
        os << "USERDEF";
        break;
    case 5:
        os << "POINTER";
        break;
    }

    return os;
}

#endif
