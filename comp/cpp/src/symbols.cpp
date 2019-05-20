#include "comp/cpp/include/symbols.h"

using namespace Symbols;
using Containers::Table;
using Inter::Id;
using CompLexer::Token;

Env::Env() :
    m_tab(new Table<Id>), m_prev(nullptr)
{
}

Env::~Env()
{
    delete m_prev;
    delete m_tab;
}


void
Env::set(Token *w, Id *i)
{
    m_tab = m_tab->set(i, w->val());
}

Id *
Env::get(Token *w)
{
    for (Env *e = this; e != nullptr; e = e->m_prev)
    {
        Id *i = m_tab->get(w->val());
        if (i != nullptr)
        {
            return i;
        }
    }
    return nullptr;
}
