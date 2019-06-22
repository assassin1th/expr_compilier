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

class SymLink : public Sym
{
public:
    SymLink(const CompLexer::Token *tok, SymTag tag = SymTag::UNDEFINED);
    virtual ~SymLink();
private:
    const SymTag m_tag;
};

class DefinedSymLink : public SymLink
{
public:
    DefinedSymLink(const CompLexer::Token *tok, int16_t offset);
    virtual ~DefinedSymLink();
    virtual const std::string gen() const;
private:
    const int16_t m_offset;
};

}


#endif // LINKER_INTER_H_
