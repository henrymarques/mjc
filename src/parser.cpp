#include "parser.h"

#include <sstream>

Parser::~Parser()
{
    delete scanner;
    delete symTable;
}

Parser::Parser()
{
    symTable = new SymbolTable();
    scanner = new Scanner(symTable);
    lToken = scanner->scan();
}

void Parser::run()
{
    program();
    advance();
    auto err = error.str();
    if (err == "")
    {
        std::cout << "Compilação encerrada com sucesso!\n";
    }
    else
    {
        std::cout << "Compilação encerrada erros.\n";
    }
}

void Parser::program()
{
    mainClass();
    while (nextIs(CLASS))
    {
        classDeclaration();
    }
    match(END_OF_FILE);
}

void Parser::mainClass()
{
    match(CLASS);
    match(ID);
    match(LCBRAC);
    match(PUBLIC);
    match(STATIC);
    match(VOID);
    match(MAIN);
    match(LPAREN);
    match(STRING);
    match(LBRACE);
    match(RBRACE);
    match(ID);
    match(RPAREN);
    match(LCBRAC);
    statement();
    match(RCBRAC);
    match(RCBRAC);
}

void Parser::classDeclaration()
{
    match(CLASS);

    auto symbol = symTable->find(lToken->lexeme);
    if (symbol != nullptr)
    {
        stringstream error;
        error << "Redefinição de símbolo " << lToken->lexeme << " na linha " << scanner->getLine() << '\n';
        this->error << error.str();
        std::cout << error.str();
    }
    else
    {
        symTable->add(STEntry{*lToken, STEntry::Types::USERDEF});
    }

    match(ID);

    if (nextIs(EXTENDS))
    {
        match(EXTENDS);
        match(ID);
    }
    match(LCBRAC);

    SymbolTable* saved = symTable;
    symTable = new SymbolTable(symTable);

    while (!nextIs(RCBRAC))
    {
        if (nextIs(PUBLIC))
        {
            methodDeclaration();
        }
        else
        {
            varDeclaration();
        }
    }
    match(RCBRAC);

    delete symTable;
    symTable = saved;
}

STEntry::Types Parser::type()
{
    STEntry::Types varType;
    if (nextIs(INT))
    {
        match(INT);
        if (nextIs(LBRACE))
        {
            match(LBRACE);
            match(RBRACE);
            varType = STEntry::Types::INTARRAY;
        }
        else
        {
            varType = STEntry::Types::INT;
        }
    }
    else if (nextIs(BOOLEAN))
    {
        match(BOOLEAN);
        varType = STEntry::Types::BOOL;
    }
    else
    {
        auto symbol = symTable->find(lToken->lexeme);
        if (symbol == nullptr)
        {
            varType = STEntry::Types::NONE;
            stringstream error;
            error << "Símbolo " << lToken->lexeme << " não definido na linha " << scanner->getLine() << '\n';
            this->error << error.str();
            std::cout << error.str();
        }
        else
        {
            varType = STEntry::Types::USERDEF;
        }
        match(ID);
    }

    return varType;
}

void Parser::varDeclaration()
{
    STEntry::Types varType = type();

    auto symbol = symTable->findScope(lToken->lexeme);
    if (symbol != nullptr)
    {
        stringstream error;
        error << "Redefinição de símbolo " << lToken->lexeme << " na linha " << scanner->getLine() << '\n';
        this->error << error.str();
        std::cout << error.str();
    }
    symTable->add(STEntry{*lToken, varType});

    match(ID);
    match(SEMI);
}

void Parser::methodDeclaration()
{
    SymbolTable* saved = symTable;
    symTable = new SymbolTable(symTable);

    match(PUBLIC);
    STEntry::Types methodType = type();

    STEntry symbol{*lToken, methodType};
    symTable->add(symbol);

    match(ID);
    match(LPAREN);
    if (nextIs(INT) || nextIs(BOOLEAN) || nextIs(ID))
    {
        STEntry::Types varType = type();

        auto symbol = symTable->findScope(lToken->lexeme);
        if (symbol != nullptr)
        {
            stringstream error;
            error << "Redefinição de símbolo " << lToken->lexeme << " na linha " << scanner->getLine() << '\n';
            this->error << error.str();
            std::cout << error.str();
        }
        symTable->add(STEntry{*lToken, varType});

        match(ID);
        while (nextIs(COMMA))
        {
            match(COMMA);
            STEntry::Types varType = type();

            auto symbol = symTable->findScope(lToken->lexeme);
            if (symbol != nullptr)
            {
                stringstream error;
                error << "Redefinição de símbolo " << lToken->lexeme << " na linha " << scanner->getLine() << '\n';
                this->error << error.str();
                std::cout << error.str();
            }
            symTable->add(STEntry{*lToken, varType});
            match(ID);
        }
    }
    match(RPAREN);
    match(LCBRAC);

    while (recTipos())
    {
        varDeclaration();
    }
    while (!nextIs(RETURN))
    {
        if (nextIs(RCBRAC))
        {
            break;
        }
        statement();
    }
    match(RETURN);
    auto returnType = expression();
    match(SEMI);
    match(RCBRAC);

    if (methodType != returnType)
    {
        stringstream error;
        error << "Tipo de retorno não corresponde ao tipo da função " << symbol.token.lexeme << " na linha " << scanner->getLine() << '\n';
        this->error << error.str();
        std::cout << error.str();
    }

    delete symTable;
    symTable = saved;
}

void Parser::statement()
{
    SymbolTable* saved = symTable;
    symTable = new SymbolTable(symTable);

    if (nextIs(LCBRAC))
    {
        match(LCBRAC);
        while (!nextIs(RCBRAC))
        {
            statement();
        }
        match(RCBRAC);
    }
    else if (nextIs(IF))
    {
        match(IF);
        match(LPAREN);
        auto t = expression();
        if (t != STEntry::Types::BOOL)
        {
            stringstream error;
            error << "Esperado BOOL recebido " << t << " na linha " << scanner->getLine() << '\n';
            this->error << error.str();
            std::cout << error.str();
        }
        match(RPAREN);
        statement();
        match(ELSE);
        statement();
    }
    else if (nextIs(WHILE))
    {
        match(WHILE);
        match(LPAREN);
        auto t = expression();
        if (t != STEntry::Types::BOOL)
        {
            stringstream error;
            error << "Esperado BOOL recebido " << t << " na linha " << scanner->getLine() << '\n';
            this->error << error.str();
            std::cout << error.str();
        }
        match(RPAREN);
        statement();
    }
    else if (nextIs(PRINTLN))
    {
        match(PRINTLN);
        match(LPAREN);
        expression();
        match(RPAREN);
        match(SEMI);
    }
    else
    {
        auto symbol = symTable->find(lToken->lexeme);
        STEntry::Types ltype = STEntry::Types::NONE;

        if (symbol == nullptr)
        {
            stringstream error;
            error << "Símbolo não definido " << lToken->lexeme << " na linha " << scanner->getLine() << '\n';
            this->error << error.str();
            std::cout << error.str();
        }
        else
        {
            ltype = symbol->type;
        }

        match(ID);
        if (nextIs(LBRACE))
        {
            match(LBRACE);
            auto t = expression();
            if (t != STEntry::Types::INT)
            {
                stringstream error;
                error << "Esperado INT recebido " << t << " na linha " << scanner->getLine() << '\n';
                this->error << error.str();
                std::cout << error.str();
            }
            match(RBRACE);
        }
        match(ATTRIB);
        auto rtype = expression();
        if (rtype != ltype)
        {
            stringstream error;
            error << "Esperado " << ltype << " recebido " << rtype << " na linha " << scanner->getLine() << '\n';
            this->error << error.str();
            std::cout << error.str();
        }
        match(SEMI);
    }

    delete symTable;
    symTable = saved;
}

STEntry::Types Parser::expression()
{
    STEntry::Types expType = STEntry::Types::NONE;

    if (nextIs(INTEGER_LITERAL))
    {
        expType = STEntry::Types::INT;
        match(INTEGER_LITERAL);
    }
    else if (nextIs(TRUE))
    {
        expType = STEntry::Types::BOOL;
        match(TRUE);
    }
    else if (nextIs(FALSE))
    {
        expType = STEntry::Types::BOOL;
        match(FALSE);
    }
    else if (nextIs(ID))
    {
        auto symbol = symTable->find(lToken->lexeme);
        if (symbol == nullptr)
        {
            expType = STEntry::Types::NONE;
            stringstream error;
            error << "Símbolo " << lToken->lexeme << " não definido na linha " << scanner->getLine() << '\n';
            this->error << error.str();
            std::cout << error.str();
        }
        else
        {
            expType = symbol->type;
        }

        match(ID);
    }
    else if (nextIs(THIS))
    {
        expType = STEntry::Types::USERDEF;
        match(THIS);
    }
    else if (nextIs(NEW))
    {
        expType = STEntry::Types::POINTER;
        match(NEW);
        if (nextIs(INT))
        {
            match(INT);
            match(LBRACE);
            auto t = expression();
            if (t != STEntry::Types::INT)
            {
                stringstream error;
                error << "Esperado INT recebido " << t << " na linha " << scanner->getLine() << '\n';
                this->error << error.str();
                std::cout << error.str();
            }
            match(RBRACE);
        }
        else
        {
            match(ID);
            match(LPAREN);
            match(RPAREN);
        }
    }
    else if (nextIs(OP) && lToken->lexeme == "!")
    {
        match(OP);
        auto t = expression();
        if (t != STEntry::Types::BOOL)
        {
            stringstream error;
            error << "Esperado BOOLEAN recebido " << t << " na linha " << scanner->getLine() << '\n';
            this->error << error.str();
            std::cout << error.str();
        }
    }
    else if (nextIs(LPAREN))
    {
        match(LPAREN);
        expression();
        match(RPAREN);
    }
    expressionLinha(expType);

    return expType;
}

void Parser::expressionLinha(STEntry::Types expType)
{
    if (nextIs(OP))
    {
        string op = lToken->lexeme;

        match(OP);

        auto t = expression();
        if (op == "+" || op == "-" || op == "*" || op == "/" || op == "<" || op == ">" || op == "==" || op == "!=")
        {
            if (expType != t || t != STEntry::Types::INT)
            {
                stringstream error;
                error << "Expressões precisam ter tipo INT. Recebido " << t << " e " << expType << " na linha " << scanner->getLine() << '\n';
                this->error << error.str();
                std::cout << error.str();
            }
        }
        else if (op == "&&")
        {
            if (expType != t || t != STEntry::Types::BOOL)
            {
                stringstream error;
                error << "Expressões precisam ter tipo BOOL. Recebido " << t << " e " << expType << " na linha " << scanner->getLine() << '\n';
                this->error << error.str();
                std::cout << error.str();
            }
        }
        else
        {
            stringstream error;
            error << "Operador não reconhecido " << lToken->lexeme << " na linha " << scanner->getLine() << '\n';
            this->error << error.str();
            std::cout << error.str();
        }
    }
    else if (nextIs(LBRACE))
    {
        match(LBRACE);
        expression();
        match(RBRACE);
        expressionLinha(STEntry::Types::NONE);
    }
    else if (nextIs(DOT))
    {
        match(DOT);
        if (nextIs(LENGTH))
        {
            match(LENGTH);
        }
        else
        {
            match(ID);
            if (nextIs(LPAREN))
            {
                match(LPAREN);
                if (!nextIs(RPAREN))
                {
                    expression();
                    while (nextIs(COMMA))
                    {
                        match(COMMA);
                        expression();
                    }
                }
                match(RPAREN);
            }
        }
        expressionLinha(STEntry::Types::NONE);
    }
}