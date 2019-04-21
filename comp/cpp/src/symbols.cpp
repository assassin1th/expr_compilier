#include "comp/cpp/include/symbols.h"

Env::Env() :
    tab(new Table<Id>), prev(nullptr)
{
}

Env::~Env()
{
    delete prev;
    delete tab;
}


void Env::set(Token *w, Id *i)
{
    tab = tab->set(i, w->val());
}

Id *Env::get(Token *w)
{
    for (Env *e = this; e != nullptr; e = e->prev)
    {
        Id *i = tab->get(w->val());
        if (i != nullptr)
        {
            return i;
        }
    }
    return nullptr;
}
