#include "linker/cpp/include/linkerlexer.h"

using namespace LinkerLexer;

Lexer::Lexer(const std::string &src) :
    CompLexer::Lexer(src)
{
}

Lexer::~Lexer()
{
}

CompLexer::Token *
Lexer::scan()
{
    std::string buffer;
}
