#include "asm/cpp/include/asmobject.h"
#include "asm/cpp/include/asmlexer.h"
#include <string>
#include <iostream>
#include "vm/c/include/cmd.h"

using namespace AsmInter;

Stmt::Stmt()
{
}

Stmt::~Stmt()
{
}

const std::string
Stmt::gen() const
{
    return "";
}


const std::shared_ptr<const Stmt>
Stmt::reduce(const AsmObject::SymTable *st,
             int16_t global_offset) const
{
    return std::shared_ptr<const Stmt> (new Stmt(*this));
}

size_t
Stmt::size() const
{
    return 0;
}

Seq::Seq(const std::shared_ptr<const Stmt> &stmt1,
         const std::shared_ptr<const Stmt> &stmt2) :
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

const std::shared_ptr<const Stmt>
Seq::reduce(const AsmObject::SymTable *st,
            int16_t global_offset) const
{
    const std::shared_ptr<const Stmt> stmt1 (m_stmt1->reduce(st, global_offset));
    global_offset += m_stmt1->size();
    const std::shared_ptr<const Stmt> stmt2 (m_stmt2->reduce(st, global_offset));
    return std::shared_ptr<const Stmt> (new Seq(stmt1, stmt2));
}

size_t
Seq::size() const
{
    return m_stmt1->size() + m_stmt2->size();
}

TmpSeq::TmpSeq(const std::shared_ptr<const Stmt> &stmt1,
               const std::shared_ptr<const Stmt> &stmt2) :
    Seq(stmt1, stmt2)
{
}

TmpSeq::~TmpSeq()
{
}

const std::shared_ptr<const Stmt>
TmpSeq::reduce(const AsmObject::SymTable *st,
               int16_t global_offset) const
{
    return std::shared_ptr<const Stmt>(this);
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

const std::shared_ptr<const Stmt>
Expr::reduce(const AsmObject::SymTable *st,
             int16_t global_offset) const
{
    return std::shared_ptr<const Stmt> (new Expr(*this));
}

Op::Op(const std::shared_ptr<const CompLexer::Token> &tok) :
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

const std::shared_ptr<const Stmt>
Op::reduce(const AsmObject::SymTable *st,
             int16_t global_offset) const
{
    return std::shared_ptr<const Stmt> (new Op(*this));
}

Real::Real(const std::shared_ptr<const CompLexer::Token> &tok) :
    Op(tok)
{
}

Real::~Real()
{
}

double
Real::val() const
{
    std::cerr << gen() << std::endl;
    return std::stod(gen());
}

const std::shared_ptr<const Stmt>
Real::reduce(const AsmObject::SymTable *st,
             int16_t global_offset) const
{
    return std::shared_ptr<const Stmt> (new Real(*this));
}

Cmd::Cmd(const std::shared_ptr<const CompLexer::Token> &tok) :
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

const std::shared_ptr<const Stmt>
Cmd::reduce(const AsmObject::SymTable *st,
             int16_t global_offset) const
{
    return std::shared_ptr<const Stmt> (new Cmd(*this));
}

size_t
Cmd::size() const
{
    return sizeof (cmd_t);
}

Label::Label(const std::shared_ptr<const CompLexer::Token> &tok,
             LabelTag tag) :
    m_tag(tag), m_tok(tok)
{
}

Label::~Label()
{
}

const std::string
Label::gen() const
{
    return std::to_string(0);
}

const std::shared_ptr<const Stmt>
Label::reduce(const AsmObject::SymTable *st,
             int16_t global_offset) const
{
    const std::shared_ptr<const Stmt> sym = st->get_sym(m_tok->val());
    if (sym == nullptr)
    {
        return std::shared_ptr<const Stmt> (new Label(*this));
    }
    return sym;
}

LabelTag
Label::tag() const
{
    return m_tag;
}

const std::string &
Label::id() const
{
    return m_tok->val();
}

int16_t
Label::offset() const
{
    return 0;
}

DefinedLabel::DefinedLabel(const std::shared_ptr<const CompLexer::Token> &tok,
                           int16_t offset) :
    Label(tok, LabelTag::DEFINED), m_offset(offset)
{
}

DefinedLabel::~DefinedLabel()
{
}

const std::string
DefinedLabel::gen() const
{
    return std::to_string(m_offset);
}

const std::shared_ptr<const Stmt>
DefinedLabel::reduce(const AsmObject::SymTable *st,
             int16_t global_offset) const
{
    return std::shared_ptr<const Stmt> (new DefinedLabel(*this));
}

int16_t
DefinedLabel::offset() const
{
    return m_offset;
}

LabelSeq::LabelSeq(const std::shared_ptr<const Stmt> &lbl) :
    m_lbl(lbl), m_seq(nullptr)
{
}

LabelSeq::~LabelSeq()
{
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
LabelSeq::push_label(std::shared_ptr<const Stmt> &lbl)
{
    if (m_seq)
    {
        m_seq->push_label(lbl);
    }
    else
    {
        m_seq = std::shared_ptr<LabelSeq>(new LabelSeq(lbl));
    }
}

Obj::Obj(const std::shared_ptr<const Stmt> &stmt,
         std::shared_ptr<LabelSeq> &lbl_seq) :
    m_stmt(stmt), m_lbl_seq(lbl_seq)
{
}

Obj::~Obj()
{
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

Reg::Reg(const std::shared_ptr<const CompLexer::Token> &tok) :
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

const std::shared_ptr<const Stmt>
Reg::reduce(const AsmObject::SymTable *st,
             int16_t global_offset) const
{
    return std::shared_ptr<const Stmt> (new Reg(*this));
}

Offset::Offset(const std::shared_ptr<const Expr> &offset_expr) :
    m_offset_expr(offset_expr)
{
}

Offset::~Offset()
{
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

const std::shared_ptr<const Stmt>
Offset::reduce(const AsmObject::SymTable *st,
             int16_t global_offset) const
{
    return std::shared_ptr<const Stmt> (new Offset(*this));
}

UnaryOffset::UnaryOffset(const std::shared_ptr<const CompLexer::Token> &tok,
                         const std::shared_ptr<const Offset> &offset_expr) :
    Offset(offset_expr), m_tok(tok)
{
}

UnaryOffset::~UnaryOffset()
{
}

int16_t
UnaryOffset::val() const
{
    return -std::stoi(m_offset_expr->gen());
}

const std::shared_ptr<const Stmt>
UnaryOffset::reduce(const AsmObject::SymTable *st,
             int16_t global_offset) const
{
    return std::shared_ptr<const Stmt> (new UnaryOffset(*this));
}

ArithCmd::ArithCmd(const std::shared_ptr<const CompLexer::Token> &tok) :
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

const std::shared_ptr<const Stmt>
ArithCmd::reduce(const AsmObject::SymTable *st,
             int16_t global_offset) const
{
    return std::shared_ptr<const Stmt> (new ArithCmd(*this));
}

size_t
ArithCmd::size() const
{
    return sizeof (arith_cmd_t);
}

TrigCmd::TrigCmd(const std::shared_ptr<const CompLexer::Token> &tok,
                 const std::shared_ptr<const Reg> &reg) :
    Cmd(tok), m_reg(reg)
{
}

TrigCmd::~TrigCmd()
{
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

const std::shared_ptr<const Stmt>
TrigCmd::reduce(const AsmObject::SymTable *st,
             int16_t global_offset) const
{
    return std::shared_ptr<const Stmt> (new TrigCmd(*this));
}

size_t
TrigCmd::size() const
{
    return sizeof (trig_cmd_t);
}

LoadRegCmd::LoadRegCmd(const std::shared_ptr<const CompLexer::Token> &tok,
                       const std::shared_ptr<const Reg> &reg) :
    Cmd(tok), m_reg(reg)
{
}

LoadRegCmd::~LoadRegCmd()
{
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

const std::shared_ptr<const Stmt>
LoadRegCmd::reduce(const AsmObject::SymTable *st,
             int16_t global_offset) const
{
    return std::shared_ptr<const Stmt> (new LoadRegCmd(*this));
}

size_t
LoadRegCmd::size() const
{
    return sizeof (ld_cmd_reg_t);
}

LoadMemCmd::LoadMemCmd(const std::shared_ptr<const CompLexer::Token> &tok,
                       const std::shared_ptr<const Offset> &offset) :
    Cmd(tok), m_offset(offset)
{
}

LoadMemCmd::~LoadMemCmd()
{
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

const std::shared_ptr<const Stmt>
LoadMemCmd::reduce(const AsmObject::SymTable *st,
             int16_t global_offset) const
{
    return std::shared_ptr<const Stmt> (new LoadMemCmd(*this));
}

size_t
LoadMemCmd::size() const
{
    return sizeof (ld_cmd_mem_t);
}

LoadRealCmd::LoadRealCmd(const std::shared_ptr<const CompLexer::Token> &tok,
                         const std::shared_ptr<const Real> &constant) :
    Cmd(tok), m_const(constant)
{
}

LoadRealCmd::~LoadRealCmd()
{
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

const std::shared_ptr<const Stmt>
LoadRealCmd::reduce(const AsmObject::SymTable *st,
                    int16_t global_offset) const
{
    return std::shared_ptr<const Stmt> (new LoadRealCmd(*this));
}

size_t
LoadRealCmd::size() const
{
    return sizeof (ld_cmd_real_t);
}
