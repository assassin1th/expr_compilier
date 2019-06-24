#include "linker/cpp/include/linkerobject.h"

using namespace LinkerObject;

Object::Object()
{
}

Object::~Object()
{
}

const LinkerInter::Code *
Object::solve(SymTable *st, const Objects *objs) const
{
    return new LinkerInter::Code();
}

SymTable::SymTable()
{
}

SymTable::~SymTable()
{
    for (const auto &n : m_tab)
    {
        delete n.second;
    }
}

const LinkerInter::Code *
SymTable::solve(SymTable *st, const Objects *objs) const
{
    LinkerInter::Code *code = nullptr;
    for (const auto &n : m_tab)
    {
        if (n.second->tag() == LinkerSymbols::SymTag::UNDEFINED &&
            st->get_sym(n.second) == nullptr)
        {
        }
    }
}





















































