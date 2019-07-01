#ifndef ASMINTER_H_
#define ASMINTER_H_

#ifdef __TEST__
#define __ASM_PARSER_TEST__
#endif // __TEST__

#include "comp/cpp/include/inter.h"
#include <memory>

namespace AsmInter {

class Seq : public Inter::Stmt
{
public:
    Seq(const std::shared_ptr<Stmt> &stmt1,
        const std::shared_ptr<Stmt> &stmt2);
    virtual ~Seq();
    virtual const std::string gen() const;
private:
    const std::shared_ptr<Stmt> m_stmt1;
    const std::shared_ptr<Stmt> m_stmt2;
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
    Op(const std::shared_ptr<CompLexer::Token> &tok);
    virtual ~Op();
    virtual const std::string gen() const;
protected:
    const std::shared_ptr<CompLexer::Token> m_tok;
};

class Real : public Op
{
public:
    Real(const std::shared_ptr<CompLexer::Token> &tok);
    virtual ~Real();
    double val() const;
};

class Cmd : public Inter::Stmt
{
public:
    Cmd(const std::shared_ptr<CompLexer::Token> &tok);
    virtual ~Cmd();
    virtual const std::string gen() const;
protected:
    const std::shared_ptr<CompLexer::Token> m_tok;
};

class Label : public Inter::Stmt
{
public:
    Label(const std::shared_ptr<CompLexer::Token> &tok);
    virtual ~Label();
    virtual const std::string gen() const;
protected:
    const std::shared_ptr<CompLexer::Token> m_tok;
};

class DefinedLabel : public Label
{
public:
    DefinedLabel(const std::shared_ptr<CompLexer::Token> &tok,
                 int16_t offset);
    virtual ~DefinedLabel();
    virtual const std::string gen() const;
private:
    const int16_t m_offset;
};

class LabelSeq : public Inter::Stmt
{
public:
    LabelSeq(const std::shared_ptr<Stmt> &lbl);
    virtual ~LabelSeq();
    virtual const std::string gen() const;
    void push_label(std::shared_ptr<Stmt> &lbl);
private:
    const std::shared_ptr<Stmt> m_lbl;
    std::shared_ptr<LabelSeq> m_seq;
};


class Obj : public Inter::Stmt
{
public:
    Obj(const std::shared_ptr<Stmt> &stmt,
        std::shared_ptr<LabelSeq> &lbl_seq);
    virtual ~Obj();
    virtual const std::string gen() const;
#ifdef __ASM_PARSER_TEST__
    std::string head_test() const;
    std::string cmd_test() const;
#endif // __ASM_PARSER_TEST__
private:
    const std::shared_ptr<Stmt> m_stmt;
    std::shared_ptr<LabelSeq> m_lbl_seq;
};

class Reg : public Op
{
public:
    Reg(const std::shared_ptr<CompLexer::Token> &tok);
    virtual ~Reg();
    unsigned long val() const;
};

class Offset : public Expr
{
public:
    Offset(const std::shared_ptr<Expr> &offset_expr);
    virtual ~Offset();
    virtual int16_t val() const;
    virtual const std::string gen() const;
protected:
    const std::shared_ptr<Expr> m_offset_expr;
};

class UnaryOffset : public Op, public Offset
{
public:
    UnaryOffset(const std::shared_ptr<CompLexer::Token> &tok,
                const std::shared_ptr<Offset> &offset_expr);
    virtual ~UnaryOffset();
    virtual int16_t val() const;
};

class ArithCmd : public Cmd
{
public:
    ArithCmd(const std::shared_ptr<CompLexer::Token> &tok);
    virtual ~ArithCmd();
    virtual const std::string gen() const;
};

class TrigCmd : public Cmd
{
public:
    TrigCmd(const std::shared_ptr<CompLexer::Token> &tok,
            const std::shared_ptr<Reg> &reg);
    virtual ~TrigCmd();
    virtual const std::string gen() const;
private:
    const std::shared_ptr<Reg> m_reg;
};

class LoadRegCmd : public Cmd
{
public:
    LoadRegCmd(const std::shared_ptr<CompLexer::Token> &tok,
               const std::shared_ptr<Reg> &reg);
    virtual ~LoadRegCmd();
    virtual const std::string gen() const;
private:
    const std::shared_ptr<Reg> m_reg;
};

class LoadMemCmd : public Cmd
{
public:
    LoadMemCmd(const std::shared_ptr<CompLexer::Token> &tok,
               const std::shared_ptr<Offset> &offset);
    virtual ~LoadMemCmd();
    virtual const std::string gen() const;
private:
    const std::shared_ptr<Offset> m_offset;
};

class LoadRealCmd : public Cmd
{
public:
    LoadRealCmd(const std::shared_ptr<CompLexer::Token> &tok,
                const std::shared_ptr<Real> &constant);
    virtual ~LoadRealCmd();
    virtual const std::string gen() const;
private:
    const std::shared_ptr<Real> m_const;
};

}

#endif // ASMINTER_H_
