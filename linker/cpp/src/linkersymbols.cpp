#include "linker/cpp/include/linkersymbols.h"

using namespace LinkerSymbols;

SymLink::SymLink(const CompLexer::Token *tok,
                 LinkerInter::SymTag tag) :
    Sym(tok), m_tag(tag)
{
}

SymLink::~SymLink()
{
}

DefinedSymLink::DefinedSymLink(const CompLexer::Token *tok, int16_t offset) :
    SymLink(tok, LinkerInter::SymTag::DEFINED), m_offset(offset)
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
