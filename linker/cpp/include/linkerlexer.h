#ifndef LINKER_LEXER_H_
#define LINKER_LEXER_H_

#include <string>
#include "comp/cpp/include/lexer.h"
#include "cont/cpp/include/containers.h"

namespace LinkerLexer {

enum Tag
{
    ID = 256
};

class Lexer : CompLexer::Lexer
{
public:
    Lexer(const std::string &src);
    virtual ~Lexer();
    virtual CompLexer::Token *scan();
};

}

#endif // LINKER_LEXER_H_
