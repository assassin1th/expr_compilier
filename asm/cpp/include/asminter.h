#ifndef ASMINTER_H_
#define ASMINTER_H_

#include "comp/cpp/include/inter.h"

namespace AsmInter {

class Seq : public Inter::Stmt
{
public:
    Seq(Stmt *stmt1, Stmt *stmt2);
    virtual ~Seq();
    virtual std::string gen() const;
private:
    Stmt *m_stmt1;
    Stmt *m_stmt2;
};

class Cmd : public Inter::Stmt
{
public:
    Cmd(const std::string &opcode);
    virtual ~Cmd();
    virtual std::string gen() const;
private:
    std::string m_opcode;
};

class Label : public Inter::Stmt
{
public:
    Label(CompLexer::Token *tok);
    virtual ~Label();
    virtual std::string gen() const;
protected:
    CompLexer::Token *m_tok;
};

class UndefLabel : public Label
{
public:
    UndefLabel(CompLexer::Token *tok);
    virtual ~UndefLabel();
    virtual std::string gen() const;
};

class HeaderLable : public Label
{
public:
    HeaderLable(CompLexer::Token *tok);
    virtual ~HeaderLable();
    virtual std::string gen() const;
};

class LabelSeq : public Inter::Stmt
{
public:
    LabelSeq(Stmt *lbl);
    virtual ~LabelSeq();
    virtual std::string gen() const;
    void push_label(Stmt *lbl);
private:
    Stmt *m_lbl;
    LabelSeq *m_seq;
};


class Obj : public Inter::Stmt
{
public:
    Obj(Stmt *stmt, LabelSeq *lbl_seq);
    virtual ~Obj();
    virtual std::string gen() const;
private:
    Stmt *m_stmt;
    LabelSeq *m_lbl_seq;
};

}

#endif // ASMINTER_H_
