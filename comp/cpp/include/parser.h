#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "inter.h"
#include "symbols.h"

namespace CompParser
{

class Parser
{
public:
    Parser(CompLexer::Lexer *m_lex);
    virtual ~Parser();
    virtual Inter::Expr *parse();
private:
    CompLexer::Lexer *m_lex;
    Symbols::Env *m_env;
    unsigned long used;
    void match(int tag);
    void func();
    Inter::Expr *expr();
    Inter::Expr *term();
    Inter::Expr *unary();
    Inter::Expr *factor();
    Inter::Expr *call();
    Inter::Expr *variable();
protected:
    void move();
    CompLexer::Token *m_look;
};

}
#endif // PARSER_H
