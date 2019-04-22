#ifndef INTER_H
#define INTER_H

#include "lexer.h"
#include <vector>

class Expr
{
public:
    Expr(Token *op);
    virtual ~Expr();
    virtual double calc(double *arg);
    Token *op;
};

class Id : public Expr
{
public:
    Id(Token *w, unsigned long d);
    virtual ~Id();
    virtual double calc(double *arg);
private:
    unsigned long offset;
};

class Constant : public Expr
{
public:
    Constant(Token *op);
    virtual ~Constant();
    virtual double calc(double *);
private:
    double val;
};

class Op : public Expr
{
public:
    Op(Token *op);
    virtual ~Op();
};

class Unary : public Op
{
public:
    Unary(Token *op, Expr *expr);
    virtual ~Unary();
    virtual double calc(double *arg);
private:
    Expr *expr;
};

class Arith : public Op
{
public:
    Arith(Token *op, Expr *lexpr, Expr *rexpr);
    virtual ~Arith();
    virtual double calc(double *arg);
private:
    Expr *lexpr;
    Expr *rexpr;
};

class Call : public Op
{
public:
    Call(Token *op, std::vector<Expr *> &args);
    virtual ~Call();
    virtual double calc(double *arg);
private:
    std::vector<Expr *> args;
};

#endif // INTER_H



















