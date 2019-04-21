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
    Token *look;
    Lexer *lex;
    Env *env;
    unsigned long used;
    void move();
    void match(int tag);
    void func();
    Expr *expr();
    Expr *term();
    Expr *unary();
    Expr *factor();
};

#endif // PARSER_H
