#ifndef LINKER_INTER_H_
#define LINKER_INTER_H_

#include <string>
#include "comp/cpp/include/lexer.h"

namespace LinkerObject
{
    class SymTable;
}

namespace LinkerInter
{

class Code
{
public:
    Code();
    virtual ~Code();
    virtual const std::string gen(LinkerObject::SymTable *sym,
                                  int16_t offset) const;
};

class ByteCode : public Code
{
public:
    ByteCode(const std::string &bytes);
    virtual ~ByteCode();
    virtual const std::string gen(LinkerObject::SymTable *sym,
                                  int16_t offset) const;
private:
    const std::string m_bytes;
};

class Sym : public Code
{
public:
    Sym(const CompLexer::Token *tok);
    virtual ~Sym();
    virtual const std::string gen(LinkerObject::SymTable *sym,
                                  int16_t offset) const;
protected:
    const CompLexer::Token *m_tok;
};

class SymCode : public Code
{
public:
    SymCode(const Code *code, const Sym *sym);
    virtual ~SymCode();
    virtual const std::string gen(LinkerObject::SymTable *st,
                                  int16_t offset) const;
private:
    const Code *m_code;
    const Sym *m_sym;
};

class CodeSeq : public Code
{
public:
    CodeSeq(const Code *pref_code, const Code *post_code);
    virtual ~CodeSeq();
    virtual const std::string gen(LinkerObject::SymTable *sym,
                                  int16_t offset) const;
private:
    const Code *m_pref_code;
    const Code *m_post_code;
};

}


#endif // LINKER_INTER_H_
