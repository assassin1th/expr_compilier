#include "comp/cpp/include/inter.h"

Expr::Expr(Token *op) :
    op(op)
{
}

Expr::~Expr()
{
    delete op;
}

double Expr::calc(double *arg)
{
    return 0;
}

Id::Id(Token *w, unsigned long d) :
    Expr(w), offset(d)
{
}

Id::~Id()
{
}

double Id::calc(double *arg)
{
    return arg[offset / sizeof(double)];
}

Op::Op(Token *op) :
    Expr(op)
{
}

Op::~Op()
{
}

Unary::Unary(Token *op, Expr *expr) :
    Op(op), expr(expr)
{
}

Unary::~Unary()
{
}

double Unary::calc(double *arg)
{
    switch (op->tag())
    {
        case '-':
            return -expr->calc(arg);
        default:
            return expr->calc(arg);
    }
}

Arith::Arith(Token *op, Expr *lexpr, Expr *rexpr) :
    Op(op), lexpr(lexpr), rexpr(rexpr)
{
}

Arith::~Arith()
{
}

double Arith::calc(double *arg)
{
    switch (op->tag())
    {
        case '+':
            return lexpr->calc(arg) + rexpr->calc(arg);
        case '-':
            return lexpr->calc(arg) - rexpr->calc(arg);
        case '*':
            return lexpr->calc(arg) * rexpr->calc(arg);
        case '/':
            return lexpr->calc(arg) / rexpr->calc(arg);
    }
}

Constant::Constant(Token *op) :
    Expr(op), val(atof(op->val().c_str()))
{
}

Constant::~Constant()
{
}

double Constant::calc(double *)
{
    return val;
}

Call::Call(Token *op, std::vector<Expr *> &args) :
    Op(op), args(args)
{
}

Call::~Call()
{
}

double Call::calc(double *arg)
{
    return 0.0;
}



























