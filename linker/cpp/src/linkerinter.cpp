#include "linker/cpp/include/linkerinter.h"

using namespace LinkerInter;

Code::Code()
{
}

Code::~Code()
{
}

const std::string
Code::gen() const
{
    return "";
}

const Code *
Code::reduce() const
{
    return this;
}

ByteCode::ByteCode(const std::string &bytes) :
    Code(), m_bytes(bytes)
{
}

ByteCode::~ByteCode()
{
}

const std::string
ByteCode::gen() const
{
    return m_bytes;
}

Sym::Sym(const CompLexer::Token *tok) :
    Code(), m_tok(tok)
{
}

Sym::~Sym()
{
}

const std::string
Sym::gen() const
{
    return "";
}

const Code *
Sym::reduce() const
{
    return this;
}

SymLink::SymLink(const CompLexer::Token *tok, SymTag tag) :
    Sym(tok), m_tag(tag)
{
}

SymLink::~SymLink()
{
}

DefinedSymLink::DefinedSymLink(const CompLexer::Token *tok, int16_t offset) :
    SymLink(tok, SymTag::DEFINED), m_offset(offset)
{
}

DefinedSymLink::~DefinedSymLink()
{
}

const std::string
DefinedSymLink::gen() const
{
    return std::string((char *) &m_offset, sizeof (int16_t));
}


