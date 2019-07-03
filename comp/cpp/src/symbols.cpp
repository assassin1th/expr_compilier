#include "comp/cpp/include/symbols.h"

using namespace Symbols;
using Containers::Table;
using Inter::Id;
using CompLexer::Token;

Env::Env() :
    m_prev(nullptr)
{
}

Env::~Env()
{
    delete m_prev;
}


void
Env::set(const std::shared_ptr<const Token> &w,
         const std::shared_ptr<const Id> &i)
{
    m_tab[w->val()] = i;
}

const std::shared_ptr<const Id>
Env::get(const std::shared_ptr<const Token> &w)
{
    for (Env *e = this; e != nullptr; e = e->m_prev)
    {
        TABLE_TYPE::iterator it = m_tab.find(w->val());
        if (it != m_tab.end())
        {
            return it->second;
        }
    }
    return nullptr;
}
