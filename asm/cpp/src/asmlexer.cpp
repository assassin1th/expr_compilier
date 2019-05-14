#include "asm/cpp/include/asmlexer.h"

AsmLexer::AsmLexer(const std::string src) :
    Lexer(src)
{
    reserve(new Word(MnemonicTag::END, "end"), "end");
    reserve(new Word(MnemonicTag::SUB, "fsub"), "fsub");
    reserve(new Word(MnemonicTag::SUM, "fsum"), "fsum");
    reserve(new Word(MnemonicTag::MUL, "fmul"), "fmul");
    reserve(new Word(MnemonicTag::DIV, "fdiv"), "fdiv");
    reserve(new Word(MnemonicTag::POW, "pow"), "pow");
    reserve(new Word(MnemonicTag::LOG, "log"), "log");
    reserve(new Word(MnemonicTag::SIN, "sin"), "sin");
    reserve(new Word(MnemonicTag::COS, "cos"), "cos");
    reserve(new Word(MnemonicTag::TAN, "tan"), "tan");
    reserve(new Word(MnemonicTag::CTAN, "ctan"), "ctan");
    reserve(new Word(MnemonicTag::ASIN, "asin"), "asin");
    reserve(new Word(MnemonicTag::ACOS, "acos"), "acos");
    reserve(new Word(MnemonicTag::ATAN, "atan"), "atan");
    reserve(new Word(MnemonicTag::ACT, "act"), "act");
    reserve(new Word(MnemonicTag::FLD, "fld"), "fld");
    reserve(new Word(MnemonicTag::RET, "ret"), "ret");
    reserve(new Word(MnemonicTag::PUSH, "push"), "push");
    reserve(new Word(MnemonicTag::POP, "pop"), "pop");
    reserve(new Word(MnemonicTag::CALL, "call"), "call");
}

AsmLexer::~AsmLexer()
{
}
