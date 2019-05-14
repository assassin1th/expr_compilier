#ifndef ASMLEXER_H_
#define ASMLEXER_H_

#include "comp/cpp/include/lexer.h"

enum MnemonicTag
{
    END = Tag::REAL + 1, SUB, SUM, MUL, DIV, POW, LOG, SIN,
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
