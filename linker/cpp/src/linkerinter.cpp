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
    return m_tok->val();
}

const Code *
Sym::reduce() const
{
    return this;
}
