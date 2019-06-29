#ifndef ASMINTER_H_
#define ASMINTER_H_

#ifdef __TEST__
#define __ASM_PARSER_TEST__
#endif // __TEST__

#include "comp/cpp/include/inter.h"

namespace AsmInter {

class Seq : public Inter::Stmt
{
public:
    Seq(const Stmt *stmt1, const Stmt *stmt2);
    virtual ~Seq();
    virtual const std::string gen() const;
private:
    const Stmt *m_stmt1;
    const Stmt *m_stmt2;
};

class Expr : public Inter::Stmt
{
public:
    Expr();
    virtual ~Expr();
    virtual const std::string gen() const;
};

class Op : public Expr
{
public:
    Op(const CompLexer::Token *tok);
    virtual ~Op();
    virtual const std::string gen() const;
protected:
    const CompLexer::Token *m_tok;
};

class Real : public Op
{
public:
    Real(const CompLexer::Token *tok);
    virtual ~Real();
    double val() const;
};

class Cmd : public Inter::Stmt
{
public:
    Cmd(const CompLexer::Token *tok);
    virtual ~Cmd();
    virtual const std::string gen() const;
protected:
    const CompLexer::Token *m_tok;
};

class Label : public Inter::Stmt
{
public:
    Label(const CompLexer::Token *tok);
    virtual ~Label();
    virtual const std::string gen() const;
protected:
    const CompLexer::Token *m_tok;
};

class DefinedLabel : public Label
{
public:
    DefinedLabel(const CompLexer::Token *tok, int16_t offset);
    virtual ~DefinedLabel();
    virtual const std::string gen() const;
private:
    const int16_t m_offset;
};

class LabelSeq : public Inter::Stmt
{
public:
    LabelSeq(const Stmt *lbl);
    virtual ~LabelSeq();
    virtual const std::string gen() const;
    void push_label(Stmt *lbl);
private:
    const Stmt *m_lbl;
    LabelSeq *m_seq;
};


class Obj : public Inter::Stmt
{
public:
    Obj(const Stmt *stmt, LabelSeq *lbl_seq);
    virtual ~Obj();
    virtual const std::string gen() const;
#ifdef __ASM_PARSER_TEST__
    std::string head_test() const;
    std::string cmd_test() const;
#endif // __ASM_PARSER_TEST__
private:
    const Stmt *m_stmt;
    LabelSeq *m_lbl_seq;
};

class Reg : public Op
{
public:
    Reg(const CompLexer::Token *tok);
    virtual ~Reg();
    unsigned long val() const;
};

class Offset : public Expr
{
public:
    Offset(const Expr *offset_expr);
    virtual ~Offset();
    virtual int16_t val() const;
    virtual const std::string gen() const;
protected:
    const Expr *m_offset_expr;
};

class UnaryOffset : public Op, public Offset
{
public:
    UnaryOffset(const CompLexer::Token *tok, const Offset *offset_expr);
    virtual ~UnaryOffset();
    virtual int16_t val() const;
};

class ArithCmd : public Cmd
{
public:
    ArithCmd(const CompLexer::Token *tok);
    virtual ~ArithCmd();
    virtual const std::string gen() const;
};

class TrigCmd : public Cmd
{
public:
    TrigCmd(const CompLexer::Token *tok, const Reg *reg);
    virtual ~TrigCmd();
    virtual const std::string gen() const;
private:
    const Reg *m_reg;
};

class LoadRegCmd : public Cmd
{
public:
    LoadRegCmd(const CompLexer::Token *tok, const Reg *reg);
    virtual ~LoadRegCmd();
    virtual const std::string gen() const;
private:
    const Reg *m_reg;
};

class LoadMemCmd : public Cmd
{
public:
    LoadMemCmd(const CompLexer::Token *tok, const Offset *offset);
    virtual ~LoadMemCmd();
    virtual const std::string gen() const;
private:
    const Offset *m_offset;
};

class LoadRealCmd : public Cmd
{
public:
    LoadRealCmd(const CompLexer::Token *tok, const Real *constant);
    virtual ~LoadRealCmd();
    virtual const std::string gen() const;
private:
    const Real *m_const;
};

}

#endif // ASMINTER_H_
