#include "comp/cpp/include/inter.h"
#include "test/cpp/include/test.h"
#define MIN(x, y) ((x < y) ? x : y)
using namespace Inter;
using CompLexer::Token;

Stmt::Stmt()
{
}

Stmt::~Stmt()
{
}

const std::string
Stmt::gen() const
{
    TEST_MSG({}, "gen method of Inter::Stmt class");
    return "";
}

FuncDecl::FuncDecl(const std::shared_ptr<const Token> &id,
                   const std::shared_ptr<const Stmt> &func_expr):
    Stmt(), m_id(id), m_func_expr(func_expr)
{
}

FuncDecl::~FuncDecl()
{
}


const std::string
FuncDecl::gen() const
{
    TEST_MSG({},
             "gen method of Inter::FuncDecl class");
    return m_id->val() + ":\n" + m_func_expr->gen();
}

Expr::Expr(const std::shared_ptr<const Token> &op, int n_reg) :
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

Id::Id(const std::shared_ptr<const Token> &w, unsigned long d) :
    Expr(w, 1), m_offset(d)
{
}

Id::~Id()
{
}

const std::string
Id::gen() const
{
    TEST_MSG({},
             "gen method of Id class");
    return "\tFLD [-" + std::to_string(m_offset) + "]\n";
}

Op::Op(const std::shared_ptr<const Token> &op, int n_reg) :
    Expr(op, n_reg)
{
}

Op::~Op()
{
}

Unary::Unary(const std::shared_ptr<const Token> &op,
             const std::shared_ptr<const Expr> &expr) :
    Op(op, expr->n_reg()), m_expr(expr)
{
}

Unary::~Unary()
{
}

const std::string
Unary::gen() const
{
    TEST_MSG({},
             "gen method of Inter::Unary class");
    return m_expr->gen();
}


Arith::Arith(const std::shared_ptr<const Token> &op,
             const std::shared_ptr<const Expr> &lexpr,
             const std::shared_ptr<const Expr> &rexpr) :
    Op(op, MIN(lexpr->n_reg(), rexpr->n_reg()) + 1),
    m_lexpr(lexpr), m_rexpr(rexpr)
{
}

Arith::~Arith()
{
}

const std::string
Arith::gen() const
{
    std::string res("\t");
    switch (m_op->tag())
    {
        case '+':
            TEST_MSG(res += "FSUM",
                     "In gen method of Inter::Arith class");
            break;
        case '-':
            TEST_MSG(res += "FSUB",
                     "In gen method of Inter::Arith class");
            break;
        case '*':
            TEST_MSG(res += "FMUL",
                     "In gen method of Inter::Arith class");
            break;
        case '/':
            TEST_MSG(res += "FDIV",
                     "In gen method of Inter::Arith class");
            break;
        case '^':
            TEST_MSG(res += "FPOW",
                     "In gen method of Inter::Arith class");
            break;
        case CompLexer::Tag::LOG:
            TEST_MSG(res += "FLOG",
                     "In gen method of Inter::Arith class");
            break;
    }

    if (m_lexpr->n_reg() < m_rexpr->n_reg())
    {
        return m_rexpr->gen() + m_lexpr->gen() + res + "R\n";
    }
    return m_lexpr->gen() + m_rexpr->gen() + res + "\n";
}


Constant::Constant(const std::shared_ptr<const Token> &op) :
    Expr(op, 1), m_val(atof(op->val().c_str()))
{
}

Constant::~Constant()
{
}

const std::string
Constant::gen() const
{
    TEST_MSG({},
             "In gen method of Inter::Constant class");
    return "\tFLD " + std::to_string(m_val) + "\n";
}

Trig::Trig(const std::shared_ptr <const CompLexer::Token> &op,
           const std::shared_ptr <const Expr> &expr):
    Op(op, expr->n_reg()), m_expr(expr)
{
}

Trig::~Trig()
{
}

const std::string
Trig::gen() const
{
    using CompLexer::Tag;
    std::string res("\t");
    switch (m_op->tag())
    {
        case Tag::COS:
            TEST_MSG(res += "FCOS ST0",
                     "In gen method of Inter::Trig class");
            break;
        case Tag::SIN:
            TEST_MSG(res += "FCOS ST0",
                     "In gen method of Inter::Trig class");
            break;
        case Tag::TAN:
            TEST_MSG(res += "FCOS ST0",
                     "In gen method of Inter::Trig class");
            break;
        case Tag::CTAN:
            TEST_MSG(res += "FCOS ST0",
                     "In gen method of Inter::Trig class");
            break;
        case Tag::ACOS:
            TEST_MSG(res += "FCOS ST0",
                     "In gen method of Inter::Trig class");
            break;
        case Tag::ASIN:
            TEST_MSG(res += "FCOS ST0",
                     "In gen method of Inter::Trig class");
            break;
        case Tag::ATAN:
            TEST_MSG(res += "FCOS ST0",
                     "In gen method of Inter::Trig class");
            break;
        case Tag::ACTAN:
            TEST_MSG(res += "FCOS ST0",
                     "In gen method of Inter::Trig class");
            break;
    }
    return m_expr->gen() + res + "\n";
}

Call::Call(const std::shared_ptr<const Token> &op,
           std::vector<std::shared_ptr<const Expr> > &args) :
    Op(op), m_args(args)
{
}

Call::~Call()
{
}

const std::string
Call::gen() const
{

    TEST_MSG({},
             "In gen method of Inter::Call class");
    return "";
}
