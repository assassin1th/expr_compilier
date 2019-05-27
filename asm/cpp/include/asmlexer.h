#ifndef ASMLEXER_H_
#define ASMLEXER_H_

#include "comp/cpp/include/lexer.h"

namespace AsmLexer {

enum Tag
{
    END = CompLexer::Tag::REAL + 1, SUB, SUBR, SUM, SUMR, MUL,
    MULR, DIV, DIVR, POW, POWR, LOG, LOGR, SIN,
    COS, TAN, CTAN, ASIN, ACOS, ATAN, ACTAN, FLD,
    RET, PUSH, POP, CALL, REG
};

class Register : public CompLexer::Token
{
public:
    Register(unsigned int);
    virtual ~Register();
    virtual const std::string val() const;
private:
    unsigned int m_num;
};

class Lexer : public CompLexer::Lexer
{
public:
    Lexer(const std::string &src);
    ~Lexer();
private:

};

}
#endif // ASMLEXER_H_
