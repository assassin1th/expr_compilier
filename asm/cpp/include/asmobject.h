#ifndef ASM_OBJECT_H_
#define ASM_OBJECT_H_

#include <unordered_map>
#include <vector>
#include <memory>
#include "asminter.h"

#ifdef TABLE_TYPE

#undef TABLE_TYPE
#define TABLE_TYPE std::unordered_map<std::string, const std::shared_ptr<AsmInter::Label>>

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
                const std::shared_ptr<AsmInter::Label> &sym);
    const std::shared_ptr<AsmInter::Label> get_sym(const std::string &key) const;
    const std::shared_ptr<AsmInter::Stmt> solve(SymTable *st,
                                                const Objects *objs,
                                                int16_t global_offset,
                                                size_t block_size);
private:
    TABLE_TYPE m_tab;
};

class ObjectFile
{
public:
    ObjectFile(const SymTable *st,
               const std::shared_ptr<AsmInter::Stmt> &code);
    ~ObjectFile();
    bool find_sym(std::shared_ptr<AsmInter::Label> &sym) const;
    const std::shared_ptr<AsmInter::Stmt> solve(SymTable *st,
                                                const Objects *objs,
                                                 int16_t global_offset);
private:
    const SymTable *st;
    const std::shared_ptr<AsmInter::Stmt> code;
};

class Objects
{
public:
    Objects();
    ~Objects();
    const std::shared_ptr<ObjectFile> get_file(const std::shared_ptr<AsmInter::Label> &sym);
    int add_file(const std::shared_ptr<ObjectFile> &file);
private:
    std::vector<const std::shared_ptr<ObjectFile>> files;
};

}

#endif // ASM_OBJECT_H_
