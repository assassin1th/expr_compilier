#include "vm/c/include/cmd.h"
#include "vm/c/include/registers.h"
#include "asm/cpp/include/asmparser.h"
#include "asm/cpp/include/asminter.h"
#include "asm/cpp/include/asmlexer.h"

AsmParser::AsmParser(Lexer *lex) :
    Parser(lex)
{
}


AsmParser::~AsmParser()
{
}

Stmt *
AsmParser::stmts()
{
    if (look->tag() == '\0')
    {
        return new Stmt();
    }
    else
    {
        return new Seq(stmt(), stmts());
    }
}

Stmt *
AsmParser::stmt()
{
    switch (look->tag())
    {
        case MnemonicTag::SUM:
        {
            arith_cmd_t cmd = {.id = FSUM, .mode_flag = 0};
            return new Cmd(std::string((char *) &cmd, sizeof (arith_cmd_t)));
        }
        case MnemonicTag::SUMR:
    {
        arith_cmd_t cmd = {.id = FSUM, .mode_flag = 1};
        return new Cmd(std::string((char *) &cmd, sizeof (arith_cmd_t)));
    }
        case MnemonicTag::SUB:
    {
        arith_cmd_t cmd = {.id = FSUB, .mode_flag = 0};
        return new Cmd(std::string((char *) &cmd, sizeof (arith_cmd_t)));
    }
        case MnemonicTag::SUBR:
    {
        arith_cmd_t cmd = {.id = FSUB, .mode_flag = 1};
        return new Cmd(std::string((char *) &cmd, sizeof (arith_cmd_t)));
    }
        case MnemonicTag::MUL:
    {
        arith_cmd_t cmd = {.id = FMUL, .mode_flag = 0};
        return new Cmd(std::string((char *) &cmd, sizeof (arith_cmd_t)));
    }
        case MnemonicTag::MULR:
    {
        arith_cmd_t cmd = {.id = FMUL, .mode_flag = 1};
        return new Cmd(std::string((char *) &cmd, sizeof (arith_cmd_t)));
    }
        case MnemonicTag::DIV:
    {
        arith_cmd_t cmd = {.id = FDIV, .mode_flag = 0};
        return new Cmd(std::string((char *) &cmd, sizeof (arith_cmd_t)));
    }
        case MnemonicTag::DIVR:
    {
        arith_cmd_t cmd = {.id = FDIV, .mode_flag = 1};
        return new Cmd(std::string((char *) &cmd, sizeof (arith_cmd_t)));
    }
        case MnemonicTag::POW:
    {
        arith_cmd_t cmd = {.id = FPOW, .mode_flag = 0};
        return new Cmd(std::string((char *) &cmd, sizeof (arith_cmd_t)));
    }
        case MnemonicTag::POWR:
    {
        arith_cmd_t cmd = {.id = FPOW, .mode_flag = 1};
        return new Cmd(std::string((char *) &cmd, sizeof (arith_cmd_t)));
    }
        case MnemonicTag::LOG:
    {
        arith_cmd_t cmd = {.id = FLOG, .mode_flag = 0};
        return new Cmd(std::string((char *) &cmd, sizeof (arith_cmd_t)));
    }
        case MnemonicTag::LOGR:
    {
        arith_cmd_t cmd = {.id = FLOG, .mode_flag = 1};
        return new Cmd(std::string((char *) &cmd, sizeof (arith_cmd_t)));
    }
        case MnemonicTag::SIN:
            break;
        case MnemonicTag::COS:
            break;
        case MnemonicTag::TAN:
            break;
        case MnemonicTag::CTAN:
            break;
        case MnemonicTag::ASIN:
            break;
        case MnemonicTag::ACOS:
            break;
        case MnemonicTag::ATAN:
            break;
        case MnemonicTag::ACT:
            break;
        case MnemonicTag::FLD:
            break;
        case MnemonicTag::RET:
            break;
        case MnemonicTag::PUSH:
            break;
        case MnemonicTag::POP:
            break;
        case MnemonicTag::CALL:
            break;
    }
    return new Stmt();
}
