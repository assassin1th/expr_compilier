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
    virtual const std::string gen() const;
};


class FuncDecl : public Stmt
{
public:
    FuncDecl(const std::shared_ptr<const CompLexer::Token> &id,
             const std::shared_ptr<const Stmt> &func_expr,
             size_t n_arg);
    virtual ~FuncDecl();
    virtual const std::string gen() const;
    size_t n_arg() const;
private:
    const size_t m_n_arg;
    const std::shared_ptr<const CompLexer::Token> m_id;
    const std::shared_ptr<const Stmt> m_func_expr;
};


class Expr : public Stmt
{
public:
    Expr(const std::shared_ptr<const CompLexer::Token> &op, int n_reg = 0);
    virtual ~Expr();
    int n_reg() const;
private:
    int m_n_reg;
protected:
    const std::shared_ptr<const CompLexer::Token> m_op;
};

class Id : public Expr
{
public:
    Id(const std::shared_ptr<const CompLexer::Token> &w, unsigned long d);
    virtual ~Id();
    virtual const std::string gen() const;
protected:
    unsigned long m_offset;
};

class Constant : public Expr
{
public:
    Constant(const std::shared_ptr<const CompLexer::Token> &op);
    virtual ~Constant();
    virtual const std::string gen() const;
private:
    double m_val;
};

class Op : public Expr
{
public:
    Op(const std::shared_ptr<const CompLexer::Token> &op, int n_reg = 0);
    virtual ~Op();
};

class Unary : public Op
{
public:
    Unary(const std::shared_ptr<const CompLexer::Token> &op,
          const std::shared_ptr<const Expr> &expr);
    virtual ~Unary();
    virtual const std::string gen() const;
private:
    const std::shared_ptr<const Expr> m_expr;
};

class Arith : public Op
{
public:
    Arith(const std::shared_ptr<const CompLexer::Token> &op,
          const std::shared_ptr<const Expr> &lexpr,
          const std::shared_ptr<const Expr> &rexpr);
    virtual ~Arith();
    virtual const std::string gen() const;
private:
    const std::shared_ptr<const Expr> m_lexpr;
    const std::shared_ptr<const Expr> m_rexpr;
};

class Trig : public Op
{
public:
    Trig(const std::shared_ptr<const CompLexer::Token> &op,
         const std::shared_ptr<const Expr> &expr);
    virtual ~Trig();
    virtual const std::string gen() const;
private:
    const std::shared_ptr<const Expr> m_expr;
};

class Call : public Op
{
public:
    Call(const std::shared_ptr<const CompLexer::Token> &op,
         std::vector<std::shared_ptr<const Expr>> &args);
    virtual ~Call();
    virtual const std::string gen() const;
private:
    std::vector<std::shared_ptr<const Expr>> m_args;
};

}
#endif // INTER_H



















