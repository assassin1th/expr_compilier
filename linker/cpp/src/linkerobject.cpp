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

const LinkerSymbols::SymLink *
SymTable::get_sym(const LinkerInter::Sym *sym) const
{
    const auto &n = m_tab->find(sym->tok()->val());
    if (n == m_tab->end())
    {
        return nullptr;
    }
    else
    {
        return n.second;
    }
}

ObjectHeader::ObjectHeader(const SymTable *st) :
    m_st(st)
{
}

ObjectHeader::~ObjectHeader()
{
    delete m_st;
}

const LinkerInter::Code *
ObjectHeader::solve(SymTable *st, const Objects *objs) const
{

}

const LinkerSymbols::SymLink *
ObjectHeader::get_sym(const LinkerInter::Sym *sym) const
{
    return m_st->get_sym(sym);
}

ObjectFile::ObjectFile(const ObjectHeader *objh,
                       const LinkerInter::Code *objcode) :
    m_objh(objh), m_objcode(objcode)
{
}

ObjectFile::~ObjectFile()
{
    delete m_objh;
    delete m_objcode;
}

const LinkerInter::Code *
ObjectFile::solve(SymTable *st,
                  const Objects *objs) const
{
    return nullptr;
}

const LinkerSymbols::SymLink *
ObjectFile::find_sym(const LinkerInter::Sym *sym) const
{
    return m_objh->get_sym(sym);
}

Objects::Objects()
{
}

Objects::~Objects()
{
    for (const auto &n : m_files)
    {
        delete n;
    }
}

int
Objects::add(ObjectFile *obj_file)
{
    m_files.push_back(obj_file);
}
