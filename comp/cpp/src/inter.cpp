#include "comp/cpp/include/inter.h"
#define MIN(x, y) ((x < y) ? x : y)
using namespace Inter;
using CompLexer::Token;

Stmt::Stmt()
{
}

Stmt::~Stmt()
{
}

std::string
Stmt::gen() const
{
    return "";
}

FuncDecl::FuncDecl(Token *id):
    Stmt(), m_id(id)
{
}

FuncDecl::~FuncDecl()
{
}


std::string
FuncDecl::gen() const
{
    return m_id->val() + ":\n";
}

Expr::Expr(Token *op, int n_reg) :
    m_n_reg(n_reg), m_op(op)
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

Id::Id(Token *w, unsigned long d) :
    Expr(w, 1), m_offset(d)
{
}

Id::~Id()
{
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

std::string
Call::gen() const
{
    return "";
}
