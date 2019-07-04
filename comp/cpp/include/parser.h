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
    virtual const std::shared_ptr<const Inter::FuncDecl> parse();
private:
    CompLexer::Lexer *m_lex;
    Symbols::Env *m_env;
    unsigned long m_used;
    void match(int tag);
    const std::shared_ptr<const Inter::FuncDecl> func();
    const std::shared_ptr<const Inter::Expr> expr();
    const std::shared_ptr<const Inter::Expr> term();
    const std::shared_ptr<const Inter::Expr> unary();
    const std::shared_ptr<const Inter::Expr> factor();
    const std::shared_ptr<const Inter::Expr> call();
    const std::shared_ptr<const Inter::Expr> variable();
    const std::shared_ptr<const Inter::Expr> trig();
    const std::shared_ptr<const Inter::Expr> power();
protected:
    void move();
    std::shared_ptr<const CompLexer::Token> m_look;
};

}
#endif // PARSER_H
