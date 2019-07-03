#include <iostream>
#include "vm/c/include/cmd.h"
#include "vm/c/include/registers.h"
#include "asm/cpp/include/asmparser.h"
#include "asm/cpp/include/asminter.h"
#include "asm/cpp/include/asmlexer.h"
#define LEX_RESERVE_WORD(lex, tag, key) (lex->reserve(new CompLexer::Word(tag, key), key))
#define LEX_RESERVE_REG(lex, num, key) (lex->reserve(new AsmLexer::Register(num), key))

using namespace AsmParser;

Parser::Parser(CompLexer::Lexer *lex) :
    m_lex(lex), m_offset(0)
{
    using CompLexer::Word;
    using AsmLexer::Tag;
    // Arith cmds
    LEX_RESERVE_WORD(lex, Tag::SUB, "FSUB");
    LEX_RESERVE_WORD(lex, Tag::SUBR, "FSUBR");
    LEX_RESERVE_WORD(lex, Tag::SUM, "FSUM");
    LEX_RESERVE_WORD(lex, Tag::SUMR, "FSUMR");
    LEX_RESERVE_WORD(lex, Tag::DIV, "FDIV");
    LEX_RESERVE_WORD(lex, Tag::DIVR, "DIVR");
    LEX_RESERVE_WORD(lex, Tag::MUL, "FMUL");
    LEX_RESERVE_WORD(lex, Tag::MULR, "FMULR");
    LEX_RESERVE_WORD(lex, Tag::LOG, "FLOG");
    LEX_RESERVE_WORD(lex, Tag::LOGR, "FLOGR");
    LEX_RESERVE_WORD(lex, Tag::POW, "FPOW");
    LEX_RESERVE_WORD(lex, Tag::POWR, "FPOWR");
    // Trig cmds
    LEX_RESERVE_WORD(lex, Tag::SIN, "FSIN");
    LEX_RESERVE_WORD(lex, Tag::COS, "FCOS");
    LEX_RESERVE_WORD(lex, Tag::TAN, "FTAN");
    LEX_RESERVE_WORD(lex, Tag::CTAN, "FCTAN");
    LEX_RESERVE_WORD(lex, Tag::ASIN, "FASIN");
    LEX_RESERVE_WORD(lex, Tag::ACOS, "FACOS");
    LEX_RESERVE_WORD(lex, Tag::ATAN, "FATAN");
    LEX_RESERVE_WORD(lex, Tag::ACTAN, "FACTAN");
    // Other cmds
    LEX_RESERVE_WORD(lex, Tag::END, "END");
    LEX_RESERVE_WORD(lex, Tag::FLD, "FLD");
    LEX_RESERVE_WORD(lex, Tag::RET, "RET");
    LEX_RESERVE_WORD(lex, Tag::PUSH, "PUSH");
    LEX_RESERVE_WORD(lex, Tag::CALL, "CALL");
    LEX_RESERVE_WORD(lex, Tag::POP, "POP");
    // Registers
    LEX_RESERVE_REG(lex, 0, "ST0");
    LEX_RESERVE_REG(lex, 1, "ST1");
    LEX_RESERVE_REG(lex, 2, "ST2");
    LEX_RESERVE_REG(lex, 3, "ST3");
    LEX_RESERVE_REG(lex, 4, "ST4");
    LEX_RESERVE_REG(lex, 5, "ST5");
    LEX_RESERVE_REG(lex, 6, "ST6");
    LEX_RESERVE_REG(lex, 7, "ST7");
    move();
}

Parser::~Parser()
{
    delete m_lex;
}

void
Parser::move()
{
    m_look = m_lex->scan();
}

void
Parser::match(int tag)
{
    if (m_look->tag() == tag)
    {
        move();
    }
    else
    {
        std::cerr << "unexpected sym" << std::endl;
    }
}

const std::shared_ptr<const AsmInter::Obj>
Parser::parse()
{
    using AsmInter::LabelSeq;
    using AsmInter::Obj;
    m_lbl_seq = std::shared_ptr<LabelSeq>(new LabelSeq(label()));
    return std::shared_ptr<const AsmInter::Obj>(new Obj(stmts(), m_lbl_seq));
}

const std::shared_ptr<const AsmInter::Stmt>
Parser::label()
{
    using CompLexer::Token;
    using CompLexer::Tag;
    using AsmInter::Label;
    using AsmInter::DefinedLabel;

    std::shared_ptr<const Token> id = m_look;
    match(Tag::ID);
    if (m_look->tag() == ':')
    {
        move();
        return std::shared_ptr<AsmInter::Stmt>(new DefinedLabel(id, m_offset));
    }
    return std::shared_ptr<AsmInter::Stmt>(new Label(id));
}

const std::shared_ptr<const AsmInter::Stmt>
Parser::stmts()
{
    using AsmInter::Seq;

    if (m_look->tag() == '\0')
    {
        return std::shared_ptr<AsmInter::Stmt>(new AsmInter::Stmt());
    }
    else
    {
        return std::shared_ptr<AsmInter::Stmt>(new Seq(stmt(), stmts()));
    }
}

const std::shared_ptr<const AsmInter::Stmt>
Parser::stmt()
{
    return arith_cmd();
}

const std::shared_ptr<const AsmInter::Stmt>
Parser::arith_cmd()
{
    using AsmLexer::Tag;
    using AsmInter::ArithCmd;
    switch (m_look->tag())
    {
        case Tag::SUM: case Tag::SUMR: case Tag::SUB: case Tag::SUBR:
        case Tag::MUL: case Tag::MULR: case Tag::DIV: case Tag::DIVR:
        case Tag::LOG: case Tag::LOGR: case Tag::POW: case Tag::POWR:
            const std::shared_ptr<const AsmInter::Stmt> x (new ArithCmd(m_look));
            move();
            m_offset += sizeof (arith_cmd_t);
            return x;
    }
    return trig_cmd();
}

const std::shared_ptr<const AsmInter::Stmt>
Parser::trig_cmd()
{
    using AsmInter::TrigCmd;
    using AsmLexer::Tag;
    using CompLexer::Token;
    switch (m_look->tag())
    {
        case Tag::COS: case Tag::SIN: case Tag::TAN: case Tag::CTAN:
        case Tag::ASIN: case Tag::ACOS: case Tag::ATAN: case Tag::ACTAN:
            const std::shared_ptr<const CompLexer::Token> tok (m_look);
            move();
            m_offset += sizeof (trig_cmd_t);
            return std::shared_ptr<const AsmInter::Stmt> (new TrigCmd(tok, reg()));
    }
    return ld_cmd();
}

const std::shared_ptr<const AsmInter::Reg>
Parser::reg()
{
    using AsmLexer::Tag;
    using CompLexer::Token;
    using AsmInter::Reg;

    const std::shared_ptr<const Token> tok (m_look);
    match(Tag::REG);

    return std::shared_ptr<const AsmInter::Reg>(new Reg(tok));
}

const std::shared_ptr<const AsmInter::Stmt>
Parser::ld_cmd()
{
    using AsmLexer::Tag;
    using AsmInter::LoadRealCmd;
    using AsmInter::LoadMemCmd;
    using AsmInter::LoadRegCmd;
    using AsmInter::Real;
    using CompLexer::Token;

    switch (m_look->tag())
    {
        case Tag::FLD:
        case Tag::PUSH:
            const std::shared_ptr<const CompLexer::Token> tok (m_look);
            move();
            if (m_look->tag() == '[')
            {
                move();
                const std::shared_ptr<const AsmInter::Stmt> x (new LoadMemCmd(tok, offset()));
                match(']');
                m_offset += sizeof (ld_cmd_mem_t);
                return x;
            }
            else if (m_look->tag() == Tag::REG)
            {
                const std::shared_ptr<const AsmInter::Stmt> x (new LoadRegCmd(tok, reg()));
                m_offset += sizeof (ld_cmd_reg_t);
                move();
                return x;
            }
            else if (m_look->tag() == CompLexer::Tag::REAL)
            {
                const std::shared_ptr<const AsmInter::Stmt> x
                    (new LoadRealCmd(tok,
                                     std::shared_ptr<AsmInter::Real>(new Real(m_look))));
                m_offset += sizeof (ld_cmd_real_t);
                move();
                return x;
            }
            else
            {
                std::cerr << "unexpected sym:" << std::endl
                          << "expected offset register or real" << std::endl
                          << m_look->val() << " given" << std::endl;
            }
    }
    return cmd();
}

const std::shared_ptr<const AsmInter::Offset>
Parser::offset()
{
    using AsmInter::Offset;
    using AsmInter::UnaryOffset;
    using CompLexer::Token;
    if (m_look->tag() == '-')
    {
        const std::shared_ptr<const Token> tok (m_look);
        move();
        return std::shared_ptr<const AsmInter::Offset> (new UnaryOffset(tok, offset()));
    }
    else
    {
        return std::shared_ptr<const AsmInter::Offset> (new Offset(expr()));
    }
}

const std::shared_ptr<const AsmInter::Expr>
Parser::expr()
{
    using AsmInter::Expr;
    using AsmInter::Real;
    using CompLexer::Tag;

    std::shared_ptr<const Expr> x (nullptr);
    switch (m_look->tag())
    {
        case Tag::REAL:
            x = std::shared_ptr<const AsmInter::Real> (new Real(m_look));
            break;
        default:
            std::cerr << "unexpected sym" << std::endl
                      << "expected REAL" << std::endl;
            x = std::shared_ptr<const AsmInter::Expr> (new Expr());
            break;
    }
    return x;
}

const std::shared_ptr<const AsmInter::Stmt>
Parser::call_cmd()
{
    return cmd();
}

const std::shared_ptr<const AsmInter::Stmt>
Parser::cmd()
{
    using AsmLexer::Tag;
    using AsmInter::Cmd;
    std::shared_ptr<const AsmInter::Stmt> stmt (nullptr);
    switch (m_look->tag())
    {
        case Tag::END: case Tag::POP: case Tag::RET:
            stmt = std::shared_ptr<const AsmInter::Stmt> (new Cmd(m_look));
            m_offset += sizeof (cmd_t);
            break;
        default:
            std::cerr << "unexpected sym" << std::endl
                      << "expected cmd" << std::endl;
            stmt = std::shared_ptr<const AsmInter::Stmt> (new AsmInter::Stmt());
    }
    move();
    return stmt;
}
