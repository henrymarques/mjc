#ifndef __ERROR_H
#define __ERROR_H

#include <string>
#include <iostream>

class Error
{
protected:
    std::string msg;

public:
    Error(std::string msg) : msg(msg) {}
    virtual void what() const
    {
        std::cout << msg << std::endl;
    }
};

class LexicalError : Error
{
public:
    LexicalError(std::string msg) : Error(msg) {}

    void what() const
    {
        std::cout << "Caractere não esperado: " << msg << std::endl;
    }
};

class SyntaxError : Error
{
public:
    SyntaxError(std::string msg) : Error(msg) {}

    void what() const
    {
        std::cout << "Erro de sintaxe:\n" << msg << std::endl;
    }
};

#endif