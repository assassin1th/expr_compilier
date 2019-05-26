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

LabelSeq::LabelSeq(Label *lbl) :
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
LabelSeq::push_label(Label *lbl)
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
