#include <iostream>
#include "vm/c/include/cmd.h"
#include "vm/c/include/registers.h"
#include "asm/cpp/include/asmparser.h"
#include "asm/cpp/include/asminter.h"
#include "asm/cpp/include/asmlexer.h"
#define LEX_RESERVE_WORD(lex, tag, key) (lex->reserve(new CompLexer::Word(tag, key), key))
#define LEX_RESERVE_REG(lex, num, key) (lex->reserve(new AsmLexer::Register(num), key))

using namespace AsmParser;
using Inter::Stmt;

Parser::Parser(CompLexer::Lexer *lex) :
    m_lex(lex), m_env(new Symbols::Env)
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

Stmt *
Parser::parse()
{
    using AsmInter::LabelSeq;
    using AsmInter::Obj;
    m_lbl_seq = new LabelSeq(label());
    return new Obj(stmts(), m_lbl_seq);
}

Stmt *
Parser::label()
{
    using CompLexer::Token;
    using CompLexer::Tag;
    using AsmInter::HeaderLable;
    using AsmInter::UndefLabel;

    Token *id = m_look;
    match(Tag::ID);
    if (m_look->tag() == ':')
    {
        move();
        return new HeaderLable(id);
    }
    return new UndefLabel(id);
}

Stmt *
Parser::stmts()
{
    using AsmInter::Seq;

    if (m_look->tag() == '\0')
    {
        return new Stmt();
    }
    else
    {
        return new Seq(stmt(), stmts());
    }
}

Stmt *
Parser::stmt()
{
    return arith_cmd();
}

Stmt *
Parser::arith_cmd()
{
    using AsmLexer::Tag;
    using AsmInter::Cmd;
    arith_cmd_t cmd =
    {
        .id = 0,
        .mode_flag = 0
    };

    switch (m_look->tag())
    {
    case Tag::SUM:
        cmd.id = cmd::FSUM;
        move();
        return new AsmInter::Cmd(std::string((char *) &cmd, sizeof (arith_cmd_t)));
    case Tag::SUMR:
        cmd.id = cmd::FSUM;
        cmd.mode_flag = 1;
        move();
        return new Cmd(std::string((char *) &cmd, sizeof (arith_cmd_t)));
    case Tag::SUB:
        cmd.id = cmd::FSUB;
        move();
        return new Cmd(std::string((char *) &cmd, sizeof (arith_cmd_t)));
    case Tag::SUBR:
        cmd.id = cmd::FSUB;
        cmd.mode_flag = 1;
        move();
        return new Cmd(std::string((char *) &cmd, sizeof (arith_cmd_t)));
    case Tag::MUL:
        cmd.id = cmd::FMUL;
        move();
        return new Cmd(std::string((char *) &cmd, sizeof (arith_cmd_t)));
    case Tag::MULR:
        cmd.id = cmd::FMUL;
        cmd.mode_flag = 1;
        move();
        return new Cmd(std::string((char *) &cmd, sizeof (arith_cmd_t)));
    case Tag::DIV:
        cmd.id = cmd::FDIV;
        move();
        return new Cmd(std::string((char *) &cmd, sizeof (arith_cmd_t)));
    case Tag::DIVR:
        cmd.id = cmd::FDIV;
        cmd.mode_flag = 1;
        move();
        return new Cmd(std::string((char *) &cmd, sizeof (arith_cmd_t)));
    case Tag::LOG:
        cmd.id = cmd::FLOG;
        move();
        return new Cmd(std::string((char *) &cmd, sizeof (arith_cmd_t)));
    case Tag::LOGR:
        cmd.id = cmd::FLOG;
        cmd.mode_flag = 1;
        move();
        return new Cmd(std::string((char *) &cmd, sizeof (arith_cmd_t)));
    case Tag::POW:
        cmd.id = cmd::FPOW;
        move();
        return new Cmd(std::string((char *) &cmd, sizeof (arith_cmd_t)));
    case Tag::POWR:
        cmd.id = cmd::FPOW;
        cmd.mode_flag = 1;
        move();
        return new Cmd(std::string((char *) &cmd, sizeof (arith_cmd_t)));
    }
    return trig_cmd();
}

Stmt *
Parser::trig_cmd()
{
    using AsmInter::Cmd;
    using AsmLexer::Tag;
    using CompLexer::Token;
    trig_cmd_t cmd =
    {
        .id = 0,
        .r = 0
    };

    Token *tok = nullptr;
    switch (m_look->tag())
    {
        case Tag::COS:
            cmd.id = cmd::FCOS;
            match(Tag::REG);
            return new Cmd(std::string((char *) &cmd, sizeof(trig_cmd_t)));
        case Tag::SIN:
            cmd.id = cmd::FSIN;
            match(Tag::REG);
            return new Cmd(std::string((char *) &cmd, sizeof(trig_cmd_t)));
        case Tag::TAN:
            match(Tag::REG);
            return new Cmd(std::string((char *) &cmd, sizeof(trig_cmd_t)));
        case Tag::CTAN:
            match(Tag::REG);
            return new Cmd(std::string((char *) &cmd, sizeof(trig_cmd_t)));
        case Tag::ASIN:
            match(Tag::REG);
            return new Cmd(std::string((char *) &cmd, sizeof(trig_cmd_t)));
        case Tag::ACOS:
            match(Tag::REG);
            return new Cmd(std::string((char *) &cmd, sizeof(trig_cmd_t)));
        case Tag::ATAN:
            match(Tag::REG);
            return new Cmd(std::string((char *) &cmd, sizeof(trig_cmd_t)));
        case Tag::ACTAN:
            match(Tag::REG);
            return new Cmd(std::string((char *) &cmd, sizeof(trig_cmd_t)));
    }
    return ld_cmd();
}

Stmt *
Parser::ld_cmd()
{
    using AsmLexer::Tag;
    using AsmInter::Cmd;
    using CompLexer::Token;

    int tag = 0;
    switch (m_look->tag())
    {
        case Tag::FLD:
            move();
            if ((tag = m_look->tag()) == CompLexer::REAL)
            {
                ld_cmd_real_t cmd =
                {
                    .id = cmd::FLD_REAL
                };
                *(double *) (cmd.real) = std::stod(m_look->val());
                move();
                return new Cmd(std::string((char *) &cmd, sizeof (ld_cmd_real_t)));
            }
            else if (tag == AsmLexer::REG)
            {
                ld_cmd_reg_t cmd =
                {
                    .id = cmd::FLD_REG
                };
                move();
                return new Cmd(std::string((char *) &cmd, sizeof(ld_cmd_reg_t)));
            }
            else if (tag == '[')
            {
                ld_cmd_mem_t cmd =
                {
                    .id = cmd::FLD_MEM
                };
                match(']');
                return new Cmd(std::string((char *) &cmd, sizeof (ld_cmd_mem_t)));
            }
            else
            {
                return new Cmd(std::string(""));
            }
        case Tag::PUSH:
            move();
            if ((tag = m_look->tag()) == CompLexer::Tag::REAL)
            {
                ld_cmd_real_t cmd =
                {
                    .id = cmd::PUSH_REAL
                };
                *(double *) (cmd.real) = std::stod(m_look->val());
                move();
                return new Cmd(std::string((char *) &cmd, sizeof (ld_cmd_real_t)));
            }
            else if (tag == AsmLexer::Tag::REG)
            {
                ld_cmd_reg_t cmd =
                {
                    .id = cmd::PUSH_REG
                };
                move();
                return new Cmd(std::string((char *) &cmd, sizeof (ld_cmd_reg_t)));
            }
            else if (tag == '[')
            {
                ld_cmd_mem_t cmd =
                {
                    .id = cmd::PUSH_MEM
                };
                match(']');
                return new Cmd(std::string((char *) &cmd, sizeof (ld_cmd_mem_t)));
            }
            else
            {
                return new Cmd(std::string(""));
            }
    }
    return cmd();
}

Stmt *
Parser::call_cmd()
{
    return cmd();
}

Stmt *
Parser::cmd()
{
    using AsmLexer::Tag;
    using AsmInter::Cmd;

    cmd_t cmd =
    {
        .id = 0,
        .unused_bits = 0
    };

    switch (m_look->tag())
    {
        case Tag::END:
            cmd.id = cmd::END;
            return new Cmd(std::string((char *) &cmd, sizeof (cmd_t)));
        case Tag::POP:
            cmd.id = cmd::END;
            return new Cmd(std::string((char *) &cmd, sizeof (cmd_t)));
        case Tag::RET:
            cmd.id = cmd::END;
            return new Cmd(std::string((char *) &cmd, sizeof (cmd_t)));
    }
    return new Cmd(std::string(""));
}

























