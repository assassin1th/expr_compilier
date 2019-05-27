#include "asm/cpp/include/asmlexer.h"

using namespace AsmLexer;

Register::Register(unsigned int num) :
    Token(Tag::REG), m_num(num)
{
}

Register::~Register()
{
}

const std::string
Register::val() const
{
    return std::to_string(m_num);
}

Lexer::Lexer(const std::string &src) :
    CompLexer::Lexer(src)
{
}

Lexer::~Lexer()
{
}
