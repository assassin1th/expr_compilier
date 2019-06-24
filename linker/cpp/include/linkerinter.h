#ifndef LINKER_INTER_H_
#define LINKER_INTER_H_

#include <string>
#include "comp/cpp/include/lexer.h"

namespace LinkerInter {

enum SymTag
{
    DEFINED, UNDEFINED
};


class Code
{
public:
    Code();
    virtual ~Code();
    virtual const std::string gen() const;
    virtual const Code *reduce() const;
};

class ByteCode : public Code
{
public:
    ByteCode(const std::string &bytes);
    virtual ~ByteCode();
    virtual const std::string gen() const;
private:
    const std::string m_bytes;
};

class Sym : public Code
{
public:
    Sym(const CompLexer::Token *tok);
    virtual ~Sym();
    virtual const std::string gen() const;
    virtual const Code *reduce() const;
protected:
    const CompLexer::Token *m_tok;
};


}


#endif // LINKER_INTER_H_
