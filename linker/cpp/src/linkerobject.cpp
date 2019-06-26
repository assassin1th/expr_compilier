#include "linker/cpp/include/linkerobject.h"

using namespace LinkerObject;

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
SymTable::solve(SymTable *st,
                const Objects *objs,
                size_t offset,
                size_t block_size) const
{
    LinkerInter::Code *code = nullptr;
    for (const auto &n : m_tab)
    {
        if (n.second->tag() == LinkerSymbols::SymTag::UNDEFINED)
        {
            ObjectFile *file = objs->find_object_file(n.second);
            file->solve(st, objs, offset + block_size);
        }
        else if (n.second->tag() == LinkerSymbols::SymTag::DEFINED)
        {
            st->set_sym(new LinkerSymbols::DefinedSymLink(n.second->tok(), offset));
        }
    }
}

const LinkerSymbols::SymLink *
SymTable::get_sym(const LinkerInter::Sym *sym) const
{
    const auto &n = m_tab->find(sym->tok()->val());
    if (n != m_tab->end() &&
        n.second->tag() == LinkerSymbols::SymTag::DEFINED)
    {
        return n.second;
    }
    return nullptr;
}

int
SymTable::set_sym(const LinkerSymbols::SymLink *sl)
{
    if (m_tab->find(sl->tok()->val()) == m_tab->end())
    {
        m_tab[sl->tok()->val()] = sl;
        return 0;
    }
    return -1;
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
ObjectHeader::solve(SymTable *st,
                    const Objects *objs,
                    size_t offset,
                    size_t block_size) const
{
    return m_st->solve(st, objs, offset, block_size);
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
                  const Objects *objs,
                  size_t offset) const
{
    return m_objh->solve(st, objs, offset, m_objcode->size());
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

const ObjectFile *
Objects::find_object_file(const LinkerInter::Sym *sl) const
{
    for (const auto file : m_files)
    {
        if (file->find_sym(sl) != nullptr)
        {
            return file;
        }
    }
    return nullptr;
}









