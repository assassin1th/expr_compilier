#include "asm/cpp/include/asmobject.h"

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
    else
    {
        return 1;
    }
}

const std::shared_ptr<const AsmInter::Label>
SymTable::get_sym(const std::string &key) const
{
    TABLE_TYPE::const_iterator it = m_tab.find(key);
    if (it == m_tab.end())
    {
        return nullptr;
    }
    else
    {
        return it->second;
    }
}

const std::shared_ptr<const AsmInter::Stmt>
SymTable::solve(SymTable *st,
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
            st->set_sym(n.second->id(), n.second);
        }
    }

    std::shared_ptr<const AsmInter::Stmt> code (new AsmInter::Stmt);

    for (const auto &n : undef_label_vec)
    {
        const std::shared_ptr<const AsmInter::Label> l (st->get_sym(n->id()));
        if (l == nullptr)
        {
            const std::shared_ptr<const ObjectFile> file (objs->get_file(n));
            code = std::shared_ptr<const AsmInter::Stmt>
                    (new AsmInter::Seq(code, file->solve(st,
                                                         objs,
                                                         global_offset + block_size)));
        }
    }
    return code;
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

bool
ObjectFile::find_sym(std::shared_ptr<const AsmInter::Label> &sym) const
{
    std::shared_ptr<const AsmInter::Label> finded_sym = m_st->get_sym(sym->id());
    if (finded_sym == nullptr || finded_sym->tag() == AsmInter::LabelTag::UNDEFINED)
    {
        return false;
    }
    return true;
}

const std::shared_ptr<const AsmInter::Stmt>
ObjectFile::solve(SymTable *st,
                  const Objects *objs,
                  int16_t global_offset) const
{
    return std::shared_ptr<const AsmInter::Stmt> (new AsmInter::Seq(m_code,
                                                                    m_st->solve(st,
                                                                                objs,
                                                                                global_offset,
                                                                                m_code->size())));
}




















































