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
    void what() const
    {
        std::cout << msg << std::endl;
    }
};

#endif