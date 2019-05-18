#ifndef ASMLEXER_H_
#define ASMLEXER_H_

#include "comp/cpp/include/lexer.h"

enum MnemonicTag
{
    END = Tag::REAL + 1, SUB, SUBR, SUM, SUMR, MUL,
    MULR, DIV, DIVR, POW, POWR, LOG, LOGR, SIN,
    COS, TAN, CTAN, ASIN, ACOS, ATAN, ACT, FLD,
    RET, PUSH, POP, CALL
};

class AsmLexer : public Lexer
{
public:
    AsmLexer(const std::string src);
    ~AsmLexer();
private:

};

#endif // ASMLEXER_H_
