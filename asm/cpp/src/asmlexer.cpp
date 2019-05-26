#include "asm/cpp/include/asmlexer.h"

using namespace AsmLexer;

Lexer::Lexer(const std::string &src) :
    CompLexer::Lexer(src)
{
}

Lexer::~Lexer()
{
}
