#include "linker/cpp/include/linkerinter.h"
#include "linker/cpp/include/linkerobject.h"

using namespace LinkerInter;

Code::Code()
{
}

Code::~Code()
{
}

const std::string
Code::gen(LinkerObject::SymTable *st,
          int16_t offset) const
{
    return "";
}

size_t
Code::size() const
{
    return 0;
}


ByteCode::ByteCode(const std::string &bytes) :
    Code(), m_bytes(bytes)
{
}

ByteCode::~ByteCode()
{
}

const std::string
ByteCode::gen(LinkerObject::SymTable *st,
              int16_t offset) const
{
    return m_bytes;
}

size_t
ByteCode::size() const
{
    return m_bytes.size();
}

Sym::Sym(const CompLexer::Token *tok) :
    Code(), m_tok(tok)
{
}

Sym::~Sym()
{
}

const std::string
Sym::gen(LinkerObject::SymTable *st,
         int16_t offset) const
{
    return st->get_sym(this)->gen(st, offset);
}

const CompLexer::Token *
Sym::tok() const
{
    return m_tok;
}

size_t
Sym::size() const
{
    return sizeof(int16_t);
}

SymCode::SymCode(const Code *code, const Sym *sym) :
    m_code(code), m_sym(sym)
{
}

SymCode::~SymCode()
{
    delete m_code;
    delete m_sym;
}

const std::string
SymCode::gen(LinkerObject::SymTable *st,
             int16_t offset) const
{
    return m_code->gen(st, offset) + m_sym->gen(st, offset);
}

size_t
SymCode::size() const
{
    return m_code->size() + m_sym->size();
}

CodeSeq::CodeSeq(const Code *pref_code, const Code *post_code) :
    m_pref_code(pref_code), m_post_code(post_code)
{
}

CodeSeq::~CodeSeq()
{
    delete m_pref_code;
    delete m_post_code;
}

const std::string
CodeSeq::gen(LinkerObject::SymTable *st,
    int16_t offset) const
{
    return m_pref_code->gen(st, offset) + m_post_code->gen(st, offset);
}

size_t
CodeSeq::size() const
{
    return m_pref_code->size() + m_post_code->size();
}






































