#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "inter.h"
#include "symbols.h"

class Parser
{
public:
    Parser(Lexer *lex);
    ~Parser();
    Expr *parse();
private:
    Lexer *lex;
    Env *env;
    unsigned long used;
    void match(int tag);
    void func();
    Expr *expr();
    Expr *term();
    Expr *unary();
    Expr *factor();
    Expr *call();
    Expr *variable();
protected:
    void move();
    Token *look;
};

#endif // PARSER_H
