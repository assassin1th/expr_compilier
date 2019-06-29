#include "asm/cpp/include/asminter.h"
#include "asm/cpp/include/asmlexer.h"
#include <string>
#include "vm/c/include/cmd.h"

using namespace AsmInter;

Seq::Seq(const Stmt *stmt1, const Stmt *stmt2) :
    Stmt(), m_stmt1(stmt1), m_stmt2(stmt2)
{
}

Seq::~Seq()
{
}

const std::string
Seq::gen() const
{
    return m_stmt1->gen() + m_stmt2->gen();
}

Expr::Expr()
{
}

Expr::~Expr()
{
}

const std::string
Expr::gen() const
{
    return std::string("");
}

Op::Op(const CompLexer::Token *tok) :
    m_tok(tok)
{
}

Op::~Op()
{
}

const std::string
Op::gen() const
{
    return m_tok->val();
}

Real::Real(const CompLexer::Token *tok) :
    Op(tok)
{
}

Real::~Real()
{
}

double
Real::val() const
{
    return std::stod(gen());
}

Cmd::Cmd(const CompLexer::Token *tok) :
    Stmt(), m_tok(tok)
{
}

Cmd::~Cmd()
{
}

const std::string
Cmd::gen() const
{
    using AsmLexer::Tag;

    cmd_t cmd =
    {
        .id = 0,
        .unused_bits = 0
    };

    switch (m_tok->tag())
    {
        case Tag::RET:
            cmd.id = cmd::RET;
            break;
        case Tag::END:
            cmd.id = cmd::END;
            break;
        case Tag::POP:
            cmd.id = cmd::POP;
            break;
    }
    return std::string((char *) &cmd, sizeof (cmd));
}

Label::Label(const CompLexer::Token *tok) :
    m_tok(tok)
{
}

Label::~Label()
{
}

const std::string
Label::gen() const
{
    return "\t" + m_tok->val() + "U\n";
}

DefinedLabel::DefinedLabel(const CompLexer::Token *tok,
                           int16_t offset) :
    Label(tok), m_offset(offset)
{
}

DefinedLabel::~DefinedLabel()
{
}

const std::string
DefinedLabel::gen() const
{
    return "\t" + m_tok->val() + " D " + std::to_string(m_offset) + "\n";
}

LabelSeq::LabelSeq(const Stmt *lbl) :
    m_lbl(lbl), m_seq(nullptr)
{
}

LabelSeq::~LabelSeq()
{
    delete m_lbl;
    delete m_seq;
}

const std::string
LabelSeq::gen() const
{
    if (m_seq)
    {
        return m_lbl->gen() + m_seq->gen();
    }
    else
    {
        return m_lbl->gen();
    }
}

void
LabelSeq::push_label(Stmt *lbl)
{
    if (m_seq)
    {
        m_seq->push_label(lbl);
    }
    else
    {
        m_seq = new LabelSeq(lbl);
    }
}

Obj::Obj(const Stmt *stmt, LabelSeq *lbl_seq) :
    m_stmt(stmt), m_lbl_seq(lbl_seq)
{
}

Obj::~Obj()
{
    delete m_lbl_seq;
    delete m_stmt;
}

const std::string
Obj::gen() const
{
    return "SYMTAB:\n" + m_lbl_seq->gen() + "\n" + m_stmt->gen();
}

#ifdef __ASM_PARSER_TEST__
std::string
Obj::head_test() const
{
    return "SYMTAB:\n" + m_lbl_seq->gen();
}

std::string
Obj::cmd_test() const
{
    return m_stmt->gen();
}
#endif // __ASM_PARSER_TEST__

Reg::Reg(const CompLexer::Token *tok) :
    Op(tok)
{
}

Reg::~Reg()
{
}

unsigned long
Reg::val() const
{
    return std::stoul(gen());
}

Offset::Offset(const Expr *offset_expr) :
    m_offset_expr(offset_expr)
{
}

Offset::~Offset()
{
    delete m_offset_expr;
}

int16_t
Offset::val() const
{
    return stoi(gen());
}

const std::string
Offset::gen() const
{
    return m_offset_expr->gen();
}

UnaryOffset::UnaryOffset(const CompLexer::Token *tok, const Offset *offset_expr) :
    Op(tok), Offset(offset_expr)
{
}

UnaryOffset::~UnaryOffset()
{
    delete m_offset_expr;
}

int16_t
UnaryOffset::val() const
{
    return -std::stoi(m_offset_expr->gen());
}

ArithCmd::ArithCmd(const CompLexer::Token *tok) :
    Cmd(tok)
{
}

ArithCmd::~ArithCmd()
{
}

const std::string
ArithCmd::gen() const
{
    using AsmLexer::Tag;

    arith_cmd_t cmd =
    {
        .id = 0,
        .mode_flag = 0
    };

    switch (m_tok->tag())
    {
    case Tag::SUM:
        cmd.id = cmd::FSUM;
        break;
    case Tag::SUMR:
        cmd.id = cmd::FSUM;
        cmd.mode_flag = 1;
        break;
    case Tag::SUB:
        cmd.id = cmd::FSUB;
        break;
    case Tag::SUBR:
        cmd.id = cmd::FSUB;
        cmd.mode_flag = 1;
        break;
    case Tag::DIV:
        cmd.id = cmd::FDIV;
        break;
    case Tag::DIVR:
        cmd.id = cmd::FDIV;
        cmd.mode_flag = 1;
        break;
    case Tag::MUL:
        cmd.id = cmd::FMUL;
        break;
    case Tag::MULR:
        cmd.id = cmd::FMUL;
        cmd.mode_flag = 1;
        break;
    case Tag::LOG:
        cmd.id = cmd::FLOG;
        break;
    case Tag::LOGR:
        cmd.id = cmd::FLOG;
        cmd.mode_flag = 1;
        break;
    case Tag::POW:
        cmd.id = cmd::FPOW;
        break;
    case Tag::POWR:
        cmd.id = cmd::FPOW;
        cmd.mode_flag = 1;
        break;
    }
    return std::string((char *) &cmd, sizeof (cmd));
}

TrigCmd::TrigCmd(const CompLexer::Token *tok, const Reg *reg) :
    Cmd(tok), m_reg(reg)
{
}

TrigCmd::~TrigCmd()
{
    delete m_reg;
}

const std::string
TrigCmd::gen() const
{
    using AsmLexer::Tag;

    trig_cmd_t cmd =
    {
        .id = 0,
        .r = 0
    };

    switch (m_tok->tag())
    {
        case Tag::SIN:
            cmd.id = cmd::FSIN;
            break;
        case Tag::COS:
            cmd.id = cmd::FCOS;
            break;
        case Tag::TAN:
            cmd.id = cmd::FTAN;
            break;
        case Tag::CTAN:
            cmd.id = cmd::FCTAN;
            break;
        case Tag::ASIN:
            cmd.id = cmd::FASIN;
            break;
        case Tag::ACOS:
            cmd.id = cmd::FACOS;
            break;
        case Tag::ATAN:
            cmd.id = cmd::FATAN;
            break;
        case Tag::ACTAN:
            cmd.id = cmd::FACT;
            break;
    }
    cmd.r = m_reg->val();
    return std::string((char *) &cmd, sizeof (cmd));
}

LoadRegCmd::LoadRegCmd(const CompLexer::Token *tok, const Reg *reg) :
    Cmd(tok), m_reg(reg)
{
}

LoadRegCmd::~LoadRegCmd()
{
    delete  m_reg;
}

const std::string
LoadRegCmd::gen() const
{
    using AsmLexer::Tag;
    ld_cmd_reg_t cmd;
    cmd.reg = m_reg->val();
    if (m_tok->tag() == Tag::PUSH)
    {
        cmd.id = cmd::PUSH_REG;
    }
    else
    {
        cmd.id = cmd::FLD_REG;
    }
    return std::string((char *) &cmd, sizeof (cmd));
}

LoadMemCmd::LoadMemCmd(const CompLexer::Token *tok, const Offset *offset) :
    Cmd(tok), m_offset(offset)
{
}

LoadMemCmd::~LoadMemCmd()
{
    delete m_offset;
}

const std::string
LoadMemCmd::gen() const
{
    using AsmLexer::Tag;
    ld_cmd_mem_t cmd;
    *(int16_t *) cmd.offset = m_offset->val();
    if (m_tok->tag() == Tag::PUSH)
    {
        cmd.id = cmd::PUSH_MEM;
    }
    else
    {
        cmd.id = cmd::FLD_MEM;
    }
    return std::string((char *) &cmd, sizeof (cmd));
}

LoadRealCmd::LoadRealCmd(const CompLexer::Token *tok, const Real *constant) :
    Cmd(tok), m_const(constant)
{
}

LoadRealCmd::~LoadRealCmd()
{
    delete m_const;
}

const std::string
LoadRealCmd::gen() const
{
    using AsmLexer::Tag;
    ld_cmd_real_t cmd;
    *(double *) cmd.real = m_const->val();
    if (m_tok->tag() == Tag::PUSH)
    {
        cmd.id = cmd::PUSH_REAL;
    }
    else
    {
        cmd.id = cmd::FLD_REAL;
    }
    return std::string((char *) &cmd, sizeof (cmd));
}
