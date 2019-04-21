#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "inter.h"
class Parser
{
public:
    Parser(Lexer *lex);
    ~Parser();
    Expr *parse();
private:
    Token *look;
    Lexer *lex;
    int used;
    void move();
    void match(int tag);
    Expr *expr();
    Expr *term();
    Expr *unary();
    Expr *factor();
};

#endif // PARSER_H
