#include "asm/cpp/include/asmobject.h"
#include "asm/cpp/include/asmexcept.h"
#include <iostream>
using namespace AsmObject;

SymTable::SymTable()
{
}

SymTable::~SymTable()
{
}

int
SymTable::set_sym(const std::string &key,
                  const std::shared_ptr<const AsmInter::Label> &sym)
{
    if (m_tab.find(key) == m_tab.end())
    {
        m_tab[key] = sym;
        return 0;
    }
    return 1;
}

const std::shared_ptr<const AsmInter::Label>
SymTable::get_sym() const
{
    for (const auto &n : m_tab)
    {
        if (n.second->tag() == AsmInter::LabelTag::DEFINED)
        {
            return n.second;
        }
    }
    return nullptr;
}

const std::shared_ptr<const AsmInter::Stmt>
SymTable::solve(DefinedSymTable *st,
                const Objects *objs,
                int16_t global_offset,
                size_t block_size) const
{
    std::vector<std::shared_ptr<const AsmInter::Label>> undef_label_vec;

    for (const auto &n : m_tab)
    {
        if (n.second->tag() == AsmInter::LabelTag::UNDEFINED)
        {
            undef_label_vec.push_back(n.second);
        }
        else if (n.second->tag() == AsmInter::LabelTag::DEFINED)
        {
            st->set_sym(n.second->id(),
                        std::dynamic_pointer_cast<const AsmInter::DefinedLabel>(n.second));
        }
    }

    std::shared_ptr<const AsmInter::Stmt> code (new AsmInter::Stmt);

    for (const auto &n : undef_label_vec)
    {
        const std::shared_ptr<const AsmInter::Label> l (st->get_sym(n->id()));
        if (l == nullptr)
        {
            const std::shared_ptr<const ObjectFile> file (objs->get_file(n));
            if (!file)
            {
                std::cerr << "wat? " << n->id() <<std::endl;
                continue;
            }
            code = std::shared_ptr<const AsmInter::Stmt>
                    (new AsmInter::Seq(code, file->solve(st,
                                                         objs,
                                                         global_offset + block_size)));
        }
    }
    return code;
}

DefinedSymTable::DefinedSymTable()
{
}

DefinedSymTable::~DefinedSymTable()
{
}

int
DefinedSymTable::set_sym(const std::string &key,
                         const std::shared_ptr<const AsmInter::DefinedLabel> &sym)
{
    if (m_tab.find(key) == m_tab.end())
    {
        m_tab[key] = sym;
        return 0;
    }
    return 1;
}

const std::shared_ptr<const AsmInter::DefinedLabel>
DefinedSymTable::get_sym(const std::string &key) const
{
    Table::const_iterator it = m_tab.find(key);
    if (it == m_tab.end())
    {
        return nullptr;
    }
    return it->second;
}

ObjectFile::ObjectFile(const SymTable *st,
                       const std::shared_ptr<const AsmInter::Stmt> &code) :
    m_st(st), m_code (code)
{
}

ObjectFile::~ObjectFile()
{
    delete m_st;
}

const std::shared_ptr<const AsmInter::Label>
ObjectFile::find_sym() const
{
    return m_st->get_sym();
}

const std::shared_ptr<const AsmInter::Stmt>
ObjectFile::solve(DefinedSymTable *st,
                  const Objects *objs,
                  int16_t global_offset) const
{
    return std::shared_ptr<const AsmInter::Stmt> (new AsmInter::Seq(m_code,
                                                                    m_st->solve(st,
                                                                                objs,
                                                                                global_offset,
                                                                                m_code->size())));
}

const std::string
ObjectFile::compile(const Objects *objs) const
{
    DefinedSymTable st;
    return solve(&st, objs, 0)->reduce(&st, 0)->gen();
}

Objects::Objects()
{
}

Objects::~Objects()
{
}

const std::shared_ptr<const ObjectFile>
Objects::get_file(const std::shared_ptr<const AsmInter::Label> &sym) const
{
    const Table::const_iterator &it = m_files.find(sym->id());
    if (it == m_files.end())
    {
        return nullptr;
    }
    return it->second;
}

int
Objects::add_file(const std::shared_ptr<const ObjectFile> &file)
{
    if (m_files.find(file->find_sym()->id()) != m_files.end())
    {
        throw AsmExcept::RedefiningFunc(file->find_sym()->id());
    }
    m_files[file->find_sym()->id()] = file;
    return 0;
}

int
Objects::delete_file(const std::shared_ptr<const AsmInter::Label> &sym)
{
    Table::const_iterator it = m_files.find(sym->id());
    if (it == m_files.end())
    {
        return 1;
    }
    m_files.erase(it);
    return 0;
}

size_t
Objects::n_files() const
{
    return m_files.size();
}
