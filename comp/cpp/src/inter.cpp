#include "comp/cpp/include/inter.h"

using namespace Inter;
using CompLexer::Token;

Expr::Expr(Token *op) :
    m_op(op)
{
}

Expr::~Expr()
{
}

double
Expr::calc(double *arg)
{
    return 0;
}

Id::Id(Token *w, unsigned long d) :
    Expr(w), m_offset(d)
{
}

Id::~Id()
{
}

double
Id::calc(double *arg)
{
    return arg[m_offset / sizeof(double)];
}

Op::Op(Token *op) :
    Expr(op)
{
}

Op::~Op()
{
}

Unary::Unary(Token *op, Expr *expr) :
    Op(op), m_expr(expr)
{
}

Unary::~Unary()
{
    delete m_expr;
}

double
Unary::calc(double *arg)
{
    switch (m_op->tag())
    {
        case '-':
            return -m_expr->calc(arg);
        default:
            return m_expr->calc(arg);
    }
}

Arith::Arith(Token *op, Expr *lexpr, Expr *rexpr) :
    Op(op), m_lexpr(lexpr), m_rexpr(rexpr)
{
}

Arith::~Arith()
{
    delete m_lexpr;
    delete m_rexpr;
}

double
Arith::calc(double *arg)
{
    switch (m_op->tag())
    {
        case '+':
            return m_lexpr->calc(arg) + m_rexpr->calc(arg);
        case '-':
            return m_lexpr->calc(arg) - m_rexpr->calc(arg);
        case '*':
            return m_lexpr->calc(arg) * m_rexpr->calc(arg);
        case '/':
            return m_lexpr->calc(arg) / m_rexpr->calc(arg);
    }
}

Constant::Constant(Token *op) :
    Expr(op), m_val(atof(op->val().c_str()))
{
}

Constant::~Constant()
{
}

double
Constant::calc(double *)
{
    return m_val;
}

Call::Call(Token *op, std::vector<Expr *> &args) :
    Op(op), m_args(args)
{
}

Call::~Call()
{
}

double
Call::calc(double *arg)
{
    return 0.0;
}
