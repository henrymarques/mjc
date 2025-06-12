#include "scanner.h"
#include "error.h"

#include <fstream>

extern std::ifstream fin;

Scanner::Scanner(SymbolTable* st) : st(st)
{
    peek = fin.get();
}

Token*
Scanner::scan()
{
    // we not declare a new token because
    // Scanner already have token declared as instance variable.
    stringstream lexeme;

    while (isspace(peek))
    {
        if (peek == '\n') line++;
        peek = fin.get();
    }

    if (fin.eof())
    {
        token = Token(END_OF_FILE);
        return &token;
    }
    else if (isdigit(peek))
    {
        do
        {
            lexeme << peek;
            peek = fin.get();
        } while (isdigit(peek));

        token = Token(INTEGER_LITERAL, lexeme.str());
        return &token;
    }
    else if (isalpha(peek))
    {
        do
        {
            lexeme << peek;
            peek = fin.get();
        } while (isalnum(peek) || peek == '_');

        if (lexeme.str() == "System" && peek == '.')
        {
            do
            {
                lexeme << peek;
                peek = fin.get();
            } while (isalpha(peek) || peek == '.');

            if (lexeme.str() == "System.out.println")
            {
                token = Token(PRINTLN, lexeme.str());
                return &token;
            }
            else
            {
                stringstream erro;
                erro << "System.out.println esperado na linha " << this->line;
                throw LexicalError(erro.str());
            }
        }

        string str = lexeme.str();
        STEntry* obj = st->find(str);

        token = !obj ? Token(ID, str) : Token(obj->token.type);
        return &token;
    }
    else if (peek == '<' || peek == '>' || peek == '+' || peek == '-' || peek == '*')
    {
        lexeme << peek;
        peek = fin.get();

        token = Token(OP, lexeme.str());
        return &token;
    }
    else if (peek == '/')
    {
        lexeme << peek;
        peek = fin.get();

        if (peek == '/')
        {
            while (peek != '\n' && !fin.eof())
            {
                peek = fin.get();
            }

            token = Token(COMMENT);
            return &token;
        }

        if (peek == '*')
        {
            bool flag = true;
            do
            {
                if (peek == '\n') line++;

                if (fin.eof())
                {
                    stringstream erro;
                    erro << "Final de bloco de comentário esperado na linha " << this->line;
                    throw LexicalError(erro.str());
                }

                peek = fin.get();
                if (peek == '*')
                {
                    peek = fin.get();
                    if (peek == '/') flag = false;
                }
            } while (flag);

            peek = fin.get();
            token = Token(COMMENT);
            return &token;
        }

        token = Token(OP, lexeme.str());
        return &token;
    }
    else if (peek == '!')
    {
        lexeme << peek;
        peek = fin.get();

        if (peek == '=')
        {
            lexeme << peek;
            peek = fin.get();
        }

        token = Token(OP, lexeme.str());
        return &token;
    }
    else if (peek == '&')
    {
        lexeme << peek;
        peek = fin.get();

        if (peek == '&')
        {
            lexeme << peek;
            peek = fin.get();

            token = Token(OP, lexeme.str());
            return &token;
        }
        else
        {
            stringstream erro;
            erro << "& esperado na linha " << this->line;
            throw LexicalError(erro.str());
        }
    }
    else if (peek == '=')
    {
        lexeme << peek;
        peek = fin.get();

        if (peek == '=') 
        {
            lexeme << peek;
            peek = fin.get();
        }

        token = Token(OP, lexeme.str());
        return &token;
    }
    else if (peek == '(' || peek == ')'
        || peek == '[' || peek == ']'
        || peek == '{' || peek == '}'
        || peek == ';' || peek == ',' || peek == '.')
    {
        lexeme << peek;
        peek = fin.get();

        token = Token(SEP, lexeme.str());
        return &token;
    }
    else
    {
        stringstream erro;
        erro << "Símbolo '" << peek << "' não reconhecido " << " na linha " << this->line;
        throw LexicalError(erro.str());
    }

    // simbolo invalido
}
