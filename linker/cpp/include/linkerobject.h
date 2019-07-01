#ifndef OBJECT_H_
#define OBJECT_H_

#include <unordered_map>
#include <vector>
#include "linkerinter.h"
#include "linkersymbols.h"

namespace LinkerObject {

class SymTable;
class Objects;

class SymTable
{
public:
    SymTable();
    ~SymTable();
    const LinkerInter::Code *solve(SymTable *st,
                                   const Objects *objs,
                                   size_t offset,
                                   size_t block_size) const;
    const LinkerSymbols::SymLink *get_sym(const LinkerInter::Sym *sym) const;
    int set_sym(const LinkerSymbols::SymLink *sl);
private:
    std::unordered_map<std::string, const LinkerSymbols::SymLink *> m_tab;
};

class ObjectHeader
{
public:
    ObjectHeader(const SymTable *st);
    ~ObjectHeader();
    const LinkerInter::Code *solve(SymTable *st,
                                   const Objects *objs,
                                   size_t offset,
                                   size_t block_size) const;
    const LinkerSymbols::SymLink *get_sym(const LinkerInter::Sym *sym) const;
private:
    const SymTable *m_st;
};

class ObjectFile
{
public:
    ObjectFile(const ObjectHeader *objh, const LinkerInter::Code *objcode);
    ~ObjectFile();
    const LinkerInter::Code *solve(SymTable *st,
                                           const Objects *objs,
                                           size_t offset) const;
    const LinkerSymbols::SymLink *find_sym(const LinkerInter::Sym *sym) const;
private:
    const ObjectHeader *m_objh;
    const LinkerInter::Code *m_objcode;
};

class Objects
{
public:
    Objects();
    ~Objects();
    int add(const ObjectFile *obj_file);
    const ObjectFile *find_object_file(const LinkerInter::Sym *sym) const;
private:
    std::vector<const ObjectFile *> m_files;
};

}

#endif // OBJECT_H_
