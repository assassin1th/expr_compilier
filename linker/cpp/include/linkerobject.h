#ifndef OBJECT_H_
#define OBJECT_H_

#include <unordered_map>
#include <vector>
#include "linkerinter.h"
#include "linkersymbols.h"

namespace LinkerObject {

class SymTable;
class Objects;

class Object
{
public:
    Object();
    virtual ~Object();
    virtual const LinkerInter::Code *solve(SymTable *st, const Objects *objs) const;
};

class SymTable : public Object
{
public:
    SymTable();
    virtual ~SymTable();
    virtual const LinkerInter::Code *solve(SymTable *st, const Objects *objs) const;
    const LinkerSymbols::SymLink *get_sym(const LinkerInter::Sym *sym) const;
private:
    std::unordered_map<std::string, LinkerSymbols::SymLink *> m_tab;
};

class ObjectHeader : public Object
{
public:
    ObjectHeader(const SymTable *st);
    virtual ~ObjectHeader();
    virtual const LinkerInter::Code *solve(SymTable *st, const Objects *objs) const;
    const LinkerSymbols::SymLink *get_sym(const LinkerInter::Sym *sym) const;
private:
    const SymTable *m_st;
};

class ObjectFile : public Object
{
public:
    ObjectFile(const ObjectHeader *objh, const LinkerInter::Code *objcode);
    virtual ~ObjectFile();
    virtual const LinkerInter::Code *solve(SymTable *st, const Objects *objs) const;
    const LinkerSymbols::SymLink *find_sym(LinkerInter::Sym *sym) const;
private:
    const ObjectHeader *m_objh;
    const LinkerInter::Code *m_objcode;
};

class Objects
{
public:
    Objects();
    ~Objects();
    int add(ObjectFile *obj_file);
private:
    std::vector<ObjectFile> m_files;
};

}

#endif // OBJECT_H_
