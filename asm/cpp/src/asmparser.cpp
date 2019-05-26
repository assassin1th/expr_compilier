#include <iostream>
#include "vm/c/include/cmd.h"
#include "vm/c/include/registers.h"
#include "asm/cpp/include/asmparser.h"
#include "asm/cpp/include/asminter.h"
#include "asm/cpp/include/asmlexer.h"
#define LEX_RESERVE_WORD(lex, tag, key) (lex->reserve(new Word(tag, key), key))

using namespace AsmParser;
using Inter::Stmt;

Parser::Parser(CompLexer::Lexer *lex) :
    m_lex(lex)
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
    LEX_RESERVE_WORD(lex, Tag::SIN, "SIN");
    LEX_RESERVE_WORD(lex, Tag::COS, "COS");
    LEX_RESERVE_WORD(lex, Tag::TAN, "TAN");
    LEX_RESERVE_WORD(lex, Tag::CTAN, "CTAN");
    LEX_RESERVE_WORD(lex, Tag::ASIN, "ASIN");
    LEX_RESERVE_WORD(lex, Tag::ACOS, "ACOS");
    LEX_RESERVE_WORD(lex, Tag::ATAN, "ATAN");
    LEX_RESERVE_WORD(lex, Tag::ACTAN, "ACTAN");
    // Other cmds
    LEX_RESERVE_WORD(lex, Tag::END, "END");
    LEX_RESERVE_WORD(lex, Tag::FLD, "FLD");
    LEX_RESERVE_WORD(lex, Tag::RET, "RET");
    LEX_RESERVE_WORD(lex, Tag::PUSH, "PUSH");
    LEX_RESERVE_WORD(lex, Tag::CALL, "POP");
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
    Stmt *x = label();
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
    return x;
}
