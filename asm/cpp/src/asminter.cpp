#include "asm/cpp/include/asminter.h"
#include <string>

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

Cmd::Cmd(const std::string &opcode) :
    Stmt(), m_opcode(opcode)
{
}

Cmd::~Cmd()
{
}

std::string
Cmd::gen() const
{
    return m_opcode;
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
    return m_lbl_seq->gen() + m_stmt->gen();
}






















