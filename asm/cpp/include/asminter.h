#ifndef ASMINTER_H_
#define ASMINTER_H_

#ifdef __TEST__
#define __ASM_PARSER_TEST__
#endif // __TEST__

#include "comp/cpp/include/lexer.h"
#include <memory>

namespace AsmObject
{
    class DefinedSymTable;
}

namespace AsmInter
{

enum LabelTag { DEFINED, UNDEFINED };

class Stmt
{
public:
    Stmt();
    virtual ~Stmt();
    virtual const std::string gen() const;
    virtual const std::shared_ptr<const Stmt> reduce(const AsmObject::DefinedSymTable *st,
                                               int16_t global_offset) const;
    virtual size_t size() const;
};

class Seq : public Stmt
{
public:
    Seq(const std::shared_ptr<const Stmt> &stmt1,
        const std::shared_ptr<const Stmt> &stmt2);
    virtual ~Seq();
    virtual const std::string gen() const;
    virtual const std::shared_ptr<const Stmt> reduce(const AsmObject::DefinedSymTable *st,
                                               int16_t global_offset) const;
    virtual size_t size() const;
private:
    const std::shared_ptr<const Stmt> m_stmt1;
    const std::shared_ptr<const Stmt> m_stmt2;
};

class TmpSeq : public Seq
{
public:
    TmpSeq(const std::shared_ptr<const Stmt> &stmt1,
           const std::shared_ptr<const Stmt> &stmt2);
    virtual ~TmpSeq();
    virtual const std::shared_ptr<const Stmt> reduce(const AsmObject::DefinedSymTable *st,
                                               int16_t global_offset) const;
};

class Expr : public Stmt
{
public:
    Expr();
    virtual ~Expr();
    virtual const std::string gen() const;
    virtual const std::shared_ptr<const Stmt> reduce(const AsmObject::DefinedSymTable *st,
                                               int16_t global_offset) const;
};

class Op : public Expr
{
public:
    Op(const std::shared_ptr<const CompLexer::Token> &tok);
    virtual ~Op();
    virtual const std::string gen() const;
    virtual const std::shared_ptr<const Stmt> reduce(const AsmObject::DefinedSymTable *st,
                                               int16_t global_offset) const;
protected:
    const std::shared_ptr<const CompLexer::Token> m_tok;
};

class Real : public Op
{
public:
    Real(const std::shared_ptr<const CompLexer::Token> &tok);
    virtual ~Real();
    double val() const;
    virtual const std::shared_ptr<const Stmt> reduce(const AsmObject::DefinedSymTable *st,
                                               int16_t global_offset) const;
};

class Cmd : public Stmt
{
public:
    Cmd(const std::shared_ptr<const CompLexer::Token> &tok);
    virtual ~Cmd();
    virtual const std::string gen() const;
    virtual const std::shared_ptr<const Stmt> reduce(const AsmObject::DefinedSymTable *st,
                                               int16_t global_offset) const;
    virtual size_t size() const;
protected:
    const std::shared_ptr<const CompLexer::Token> m_tok;
};

class Label : public Stmt
{
public:
    Label(const std::shared_ptr<const CompLexer::Token> &tok,
          LabelTag tag = LabelTag::UNDEFINED);
    virtual ~Label();
    virtual const std::string gen() const;
    virtual const std::shared_ptr<const Stmt> reduce(const AsmObject::DefinedSymTable *st,
                                               int16_t global_offset) const;
    LabelTag tag() const;
    const std::string &id() const;
    virtual int16_t offset() const;
protected:
    const LabelTag m_tag;
    const std::shared_ptr<const CompLexer::Token> m_tok;
};

class DefinedLabel : public Label
{
public:
    DefinedLabel(const std::shared_ptr<const CompLexer::Token> &tok,
                 int16_t offset);
    virtual ~DefinedLabel();
    virtual const std::string gen() const;
    virtual const std::shared_ptr<const Stmt> reduce(const AsmObject::DefinedSymTable *st,
                                               int16_t global_offset) const;
    virtual int16_t offset() const;
private:
    const int16_t m_offset;
};

class Reg : public Op
{
public:
    Reg(const std::shared_ptr<const CompLexer::Token> &tok);
    virtual ~Reg();
    virtual const std::shared_ptr<const Stmt> reduce(const AsmObject::DefinedSymTable *st,
                                               int16_t global_offset) const;
    unsigned long val() const;
};

class Offset : public Expr
{
public:
    Offset(const std::shared_ptr<const Expr> &offset_expr);
    virtual ~Offset();
    virtual int16_t val() const;
    virtual const std::string gen() const;
    virtual const std::shared_ptr<const Stmt> reduce(const AsmObject::DefinedSymTable *st,
                                               int16_t global_offset) const;
protected:
    const std::shared_ptr<const Expr> m_offset_expr;
};

class UnaryOffset : public Offset
{
public:
    UnaryOffset(const std::shared_ptr<const CompLexer::Token> &tok,
                const std::shared_ptr<const Offset> &offset_expr);
    virtual ~UnaryOffset();
    virtual int16_t val() const;
    virtual const std::shared_ptr<const Stmt> reduce(const AsmObject::DefinedSymTable *st,
                                               int16_t global_offset) const;
private:
    std::shared_ptr<const CompLexer::Token> m_tok;
};

class ArithCmd : public Cmd
{
public:
    ArithCmd(const std::shared_ptr<const CompLexer::Token> &tok);
    virtual ~ArithCmd();
    virtual const std::string gen() const;
    virtual const std::shared_ptr<const Stmt> reduce(const AsmObject::DefinedSymTable *st,
                                               int16_t global_offset) const;
    virtual size_t size() const;
};

class TrigCmd : public Cmd
{
public:
    TrigCmd(const std::shared_ptr<const CompLexer::Token> &tok,
            const std::shared_ptr<const Reg> &reg);
    virtual ~TrigCmd();
    virtual const std::string gen() const;
    virtual const std::shared_ptr<const Stmt> reduce(const AsmObject::DefinedSymTable *st,
                                               int16_t global_offset) const;
    virtual size_t size() const;
private:
    const std::shared_ptr<const Reg> m_reg;
};

class LoadRegCmd : public Cmd
{
public:
    LoadRegCmd(const std::shared_ptr<const CompLexer::Token> &tok,
               const std::shared_ptr<const Reg> &reg);
    virtual ~LoadRegCmd();
    virtual const std::string gen() const;
    virtual const std::shared_ptr<const Stmt> reduce(const AsmObject::DefinedSymTable *st,
                                               int16_t global_offset) const;
    virtual size_t size() const;
private:
    const std::shared_ptr<const Reg> m_reg;
};

class LoadMemCmd : public Cmd
{
public:
    LoadMemCmd(const std::shared_ptr<const CompLexer::Token> &tok,
               const std::shared_ptr<const Offset> &offset);
    virtual ~LoadMemCmd();
    virtual const std::string gen() const;
    virtual const std::shared_ptr<const Stmt> reduce(const AsmObject::DefinedSymTable *st,
                                               int16_t global_offset) const;
    virtual size_t size() const;
private:
    const std::shared_ptr<const Offset> m_offset;
};

class LoadRealCmd : public Cmd
{
public:
    LoadRealCmd(const std::shared_ptr<const CompLexer::Token> &tok,
                const std::shared_ptr<const Real> &constant);
    virtual ~LoadRealCmd();
    virtual const std::string gen() const;
    virtual const std::shared_ptr<const Stmt> reduce(const AsmObject::DefinedSymTable *st,
                                               int16_t global_offset) const;
    virtual size_t size() const;
private:
    const std::shared_ptr<const Real> m_const;
};

}

#endif // ASMINTER_H_
