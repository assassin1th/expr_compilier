#ifndef ASM_OBJECT_H_
#define ASM_OBJECT_H_

#include <unordered_map>
#include <vector>
#include <memory>
#include "asminter.h"

#ifdef TABLE_TYPE

#undef TABLE_TYPE
#define TABLE_TYPE std::unordered_map<std::string, std::shared_ptr<const AsmInter::Label>>

#else

#define TABLE_TYPE

#endif

namespace AsmObject
{

class Objects;

class SymTable
{
public:
    SymTable();
    ~SymTable();
    int set_sym(const std::string &key,
                const std::shared_ptr<const AsmInter::Label> &sym);
    const std::shared_ptr<const AsmInter::Label> get_sym(const std::string &key) const;
    const std::shared_ptr<const AsmInter::Stmt> solve(SymTable *st,
                                                      const Objects *objs,
                                                      int16_t global_offset,
                                                      size_t block_size) const;
private:
    TABLE_TYPE m_tab;
};

class ObjectFile
{
public:
    ObjectFile(const SymTable *st,
               const std::shared_ptr<const AsmInter::Stmt> &code);
    ~ObjectFile();
    bool find_sym(std::shared_ptr<const AsmInter::Label> &sym) const;
    const std::shared_ptr<const AsmInter::Stmt> solve(SymTable *st,
                                                      const Objects *objs,
                                                      int16_t global_offset) const;
private:
    const SymTable *m_st;
    const std::shared_ptr<const AsmInter::Stmt> m_code;
};

class Objects
{
public:
    Objects();
    ~Objects();
    const std::shared_ptr<const ObjectFile> get_file(const std::shared_ptr<const AsmInter::Label> &sym) const;
    int add_file(const std::shared_ptr<const ObjectFile> &file) const;
private:
    std::vector<std::shared_ptr<const ObjectFile>> files;
};

}

#endif // ASM_OBJECT_H_
