#ifndef INTER_H
#define INTER_H

#include "lexer.h"
#include <vector>

namespace Inter
{

class Stmt
{
public:
    Stmt();
    virtual ~Stmt();
    virtual std::string gen() const;
};


class FuncDecl : public Stmt
{
public:
    FuncDecl(CompLexer::Token *id);
    virtual ~FuncDecl();
    virtual std::string gen() const;
private:
    CompLexer::Token *m_id;
    Stmt *m_func_expr;
};


class Expr : public Stmt
{
public:
    Expr(CompLexer::Token *op, int n_reg = 0);
    virtual ~Expr();
    int n_reg() const;
private:
    int m_n_reg;
protected:
    CompLexer::Token *m_op;
};


class Id : public Expr
{
public:
    Id(CompLexer::Token *w, unsigned long d);
    virtual ~Id();
    virtual std::string gen() const;
private:
    unsigned long m_offset;
};

class Constant : public Expr
{
public:
    Constant(CompLexer::Token *op);
    virtual ~Constant();
    virtual std::string gen() const;
private:
    double m_val;
};

class Op : public Expr
{
public:
    Op(CompLexer::Token *op, int n_reg = 0);
    virtual ~Op();
};

class Unary : public Op
{
public:
    Unary(CompLexer::Token *op, Expr *expr);
    virtual ~Unary();
    virtual std::string gen() const;
private:
    Expr *m_expr;
};

class Arith : public Op
{
public:
    Arith(CompLexer::Token *op, Expr *lexpr, Expr *rexpr);
    virtual ~Arith();
    virtual std::string gen() const;
private:
    Expr *m_lexpr;
    Expr *m_rexpr;
};

class Trig : public Op
{
public:
    Trig(CompLexer::Token *op, Expr *expr);
    virtual ~Trig();
    virtual std::string gen() const;
private:
    Expr *m_expr;
};

class Call : public Op
{
public:
    Call(CompLexer::Token *op, std::vector<Expr *> &args);
    virtual ~Call();
    virtual std::string gen() const;
private:
    std::vector<Expr *> m_args;
};

}
#endif // INTER_H



















