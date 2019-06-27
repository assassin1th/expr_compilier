#include "linker/cpp/include/linkersymbols.h"

using namespace LinkerSymbols;

SymLink::SymLink(const CompLexer::Token *tok, SymTag tag) :
    Sym(tok), m_tag(tag)
{
}

SymLink::~SymLink()
{
}

SymTag
SymLink::tag() const
{
    return m_tag;
}

int16_t
SymLink::offset() const
{
    return 0;
}

DefinedSymLink::DefinedSymLink(const CompLexer::Token *tok, int16_t offset) :
    SymLink(tok, SymTag::DEFINED), m_offset(offset)
{
}

DefinedSymLink::~DefinedSymLink()
{
}

int16_t
DefinedSymLink::offset() const
{
    return m_offset;
}

Env::Env() :
    m_tab(new Containers::Table<SymLink>(3))
{
}

Env::~Env()
{
    delete m_tab;
}

const SymLink *
Env::get(const CompLexer::Token *tok) const
{
    return m_tab->get(tok->val());
}

void
Env::set(const CompLexer::Token *tok,
    const SymLink *sym)
{
    m_tab->set(sym, tok->val());
}
