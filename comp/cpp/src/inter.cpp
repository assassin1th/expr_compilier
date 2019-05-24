#include "comp/cpp/include/inter.h"
#define MIN(x, y) ((x < y) ? x : y)
using namespace Inter;
using CompLexer::Token;

Expr::Expr(Token *op, int n_reg) :
    m_op(op), m_n_reg(n_reg)
{
}

Expr::~Expr()
{
}

int
Expr::n_reg() const
{
    return m_n_reg;
}

double
Expr::calc(double *arg)
{
    return 0;
}

std::string
Expr::gen() const
{
    return std::string("\n");
}

Id::Id(Token *w, unsigned long d) :
    Expr(w, 1), m_offset(d)
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

std::string
Id::gen() const
{
    return "FLD [ECX - " + std::to_string(m_offset) + "]\n";
}

Op::Op(Token *op, int n_reg) :
    Expr(op, n_reg)
{
}

Op::~Op()
{
}

Unary::Unary(Token *op, Expr *expr) :
    Op(op, expr->n_reg()), m_expr(expr)
{
}

Unary::~Unary()
{
    delete m_expr;
}

std::string
Unary::gen() const
{
    return m_expr->gen();
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
    Op(op, MIN(lexpr->n_reg(), rexpr->n_reg()) + 1),
    m_lexpr(lexpr), m_rexpr(rexpr)
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

std::string
Arith::gen() const
{
    std::string res("");
    switch (m_op->tag())
    {
        case '+':
            res += "FSUM";
            break;
        case '-':
            res += "FSUB";
            break;
        case '*':
            res += "FMUL";
            break;
        case '/':
            res += "FDIV";
            break;
        case '^':
            res += "FPOW";
            break;
        case CompLexer::Tag::LOG:
            res += "FLOG";
            break;
    }

    if (m_lexpr->n_reg() > m_rexpr->n_reg())
    {
        return m_lexpr->gen() + m_rexpr->gen() + res + "\n";
    }

    return m_rexpr->gen() + m_lexpr->gen() + res + "R\n";
}


Constant::Constant(Token *op) :
    Expr(op, 1), m_val(atof(op->val().c_str()))
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
std::string
Constant::gen() const
{
    return "FLD " + std::to_string(m_val) + "\n";
}

Trig::Trig(CompLexer::Token *op, Expr *expr):
    Op(op, expr->n_reg()), m_expr(expr)
{
}

Trig::~Trig()
{
}

std::string
Trig::gen() const
{
    using CompLexer::Tag;
    std::string res("");
    switch (m_op->tag())
    {
        case Tag::COS:
            res += "FCOS\n";
            break;
        case Tag::SIN:
            res += "FSIN\n";
            break;
        case Tag::TAN:
            res += "FTAN\n";
            break;
        case Tag::CTAN:
            res += "FCTAN";
            break;
        case Tag::ACOS:
            res += "FACOS";
            break;
        case Tag::ASIN:
            res += "FASIN";
            break;
        case Tag::ATAN:
            res += "FATAN";
            break;
        case Tag::ACTAN:
            res += "FACTAN";
            break;
    }
    return m_expr->gen() + res + "\n";
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
