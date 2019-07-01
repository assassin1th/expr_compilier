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
    virtual std::shared_ptr<Inter::Stmt> parse();
private:
    CompLexer::Lexer *m_lex;
    Symbols::Env *m_env;
    unsigned long m_used;
    void match(int tag);
    std::shared_ptr<Inter::Stmt> func();
    std::shared_ptr<Inter::Expr> expr();
    std::shared_ptr<Inter::Expr> term();
    std::shared_ptr<Inter::Expr> unary();
    std::shared_ptr<Inter::Expr> factor();
    std::shared_ptr<Inter::Expr> call();
    std::shared_ptr<Inter::Expr> variable();
    std::shared_ptr<Inter::Expr> trig();
    std::shared_ptr<Inter::Expr> power();
protected:
    void move();
    std::shared_ptr<CompLexer::Token> m_look;
};

}
#endif // PARSER_H
