#include "asm/cpp/include/asminter.h"
#include "asm/cpp/include/asmlexer.h"
#include <string>
#include "vm/c/include/cmd.h"
using namespace AsmInter;

Seq::Seq(Stmt *stmt1, Stmt *stmt2) :
    Stmt(), m_stmt1(stmt1), m_stmt2(stmt2)
{
}

Seq::~Seq()
{
}

std::string
Seq::gen() const
{
    return m_stmt1->gen() + m_stmt2->gen();
}

Cmd::Cmd(CompLexer::Token *tok) :
    Stmt(), m_tok(tok)
{
}

Cmd::~Cmd()
{
}

std::string
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

Label::Label(CompLexer::Token *tok) :
    m_tok(tok)
{
}

Label::~Label()
{
}

std::string
Label::gen() const
{
    return "\t" + m_tok->val() + "D\n";
}

UndefLabel::UndefLabel(CompLexer::Token *tok) :
    Label(tok)
{
}

UndefLabel::~UndefLabel()
{
}


std::string
UndefLabel::gen() const
{
    return "\t" + m_tok->val() + "U\n";
}

HeaderLable::HeaderLable(CompLexer::Token *tok) :
    Label(tok)
{
}

HeaderLable::~HeaderLable()
{
}

std::string
HeaderLable::gen() const
{
    return "\t" + m_tok->val() + "H\n";
}

LabelSeq::LabelSeq(Stmt *lbl) :
    m_lbl(lbl), m_seq(nullptr)
{
}

LabelSeq::~LabelSeq()
{
    delete m_lbl;
    delete m_seq;
}

std::string
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

Obj::Obj(Stmt *stmt, LabelSeq *lbl_seq) :
    m_stmt(stmt), m_lbl_seq(lbl_seq)
{
}

Obj::~Obj()
{
    delete m_lbl_seq;
    delete m_stmt;
}

std::string
Obj::gen() const
{
    return "SYMTAB:" + m_lbl_seq->gen() + "\n" + m_stmt->gen();
}

Reg::Reg(CompLexer::Token *tok, unsigned long offset) :
    Inter::Id(tok, offset)
{
}

Reg::~Reg()
{
}

unsigned long
Reg::val() const
{
    return m_offset;
}

ArithCmd::ArithCmd(CompLexer::Token *tok) :
    Cmd(tok)
{
}

ArithCmd::~ArithCmd()
{
}

std::string
ArithCmd::gen() const
{
    using AsmLexer::Tag;

}

TrigCmd::TrigCmd(CompLexer::Token *tok, Reg *reg) :
    Cmd(tok), m_reg(reg)
{
}

TrigCmd::~TrigCmd()
{
}

std::string
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
























