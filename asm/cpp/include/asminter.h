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

class Expr : public Inter::Stmt
{
public:
    Expr(CompLexer::Token *tok);
    virtual ~Expr();
    virtual std::string gen() const;
protected:
    CompLexer::Token *m_tok;
};

class Real : public Expr
{
public:
    Real(CompLexer::Token *tok);
    virtual ~Real();
    double val() const;
};

class Cmd : public Inter::Stmt
{
public:
    Cmd(CompLexer::Token *tok);
    virtual ~Cmd();
    virtual std::string gen() const;
protected:
    CompLexer::Token *m_tok;
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

class Reg : public Inter::Id
{
public:
    Reg(CompLexer::Token *tok, unsigned long offset);
    virtual ~Reg();
    unsigned long val() const;
};

class Offset : public Expr
{
public:
    Offset(CompLexer::Token *tok, Expr *offset_expr);
    virtual ~Offset();
    int16_t val() const;
private:
    Expr *m_offset_expr;
};

class ArithCmd : public Cmd
{
public:
    ArithCmd(CompLexer::Token *tok);
    virtual ~ArithCmd();
    virtual std::string gen() const;
};

class TrigCmd : public Cmd
{
public:
    TrigCmd(CompLexer::Token *tok, Reg *reg);
    virtual ~TrigCmd();
    virtual std::string gen() const;
private:
    Reg *m_reg;
};

class LoadRegCmd : public Cmd
{
public:
    LoadRegCmd(CompLexer::Token *tok, Reg *reg);
    virtual ~LoadRegCmd();
    virtual std::string gen() const;
private:
    Reg *m_reg;
};
class LoadMemCmd : public Cmd
{
public:
    LoadMemCmd(CompLexer::Token *tok, Offset *offset);
    virtual ~LoadMemCmd();
    virtual std::string gen() const;
private:
    Offset *m_offset;
};

class LoadRealCmd : public Cmd
{
public:
    LoadRealCmd(CompLexer::Token *tok, Real *constant);
    virtual ~LoadRealCmd();
    virtual std::string gen() const;
private:
    Real *m_const;
};

}

#endif // ASMINTER_H_
