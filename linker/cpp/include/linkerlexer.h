#ifndef LINKER_LEXER_H_
#define LINKER_LEXER_H_

#include <string>
#include "comp/cpp/include/lexer.h"
#include "cont/cpp/include/containers.h"

namespace LinkerLexer {

enum Tag
{
    ID = 256, NUMBER
};

class Number : public CompLexer::Token
{
public:
    Number(const std::string &num);
    virtual ~Number();
    virtual const std::string &val() const;
private:
    const std::string m_num;
};

class Lexer : public CompLexer::Lexer
{
public:
    Lexer(const std::string &src);
    virtual ~Lexer();
    virtual CompLexer::Token *scan();
};

}

#endif // LINKER_LEXER_H_
