#ifndef ASM_OBJECT_H_
#define ASM_OBJECT_H_

#include <unordered_map>
#include <vector>
#include <memory>
#include "asminter.h"

namespace AsmObject
{

class Objects;

class DefinedSymTable
{
public:
    DefinedSymTable();
    ~DefinedSymTable();
    int set_sym(const std::string &key,
                const std::shared_ptr<const AsmInter::DefinedLabel> &sym);
    const std::shared_ptr<const AsmInter::DefinedLabel> get_sym(const std::string &key) const;
private:
    using Table = std::unordered_map<std::string, std::shared_ptr<const AsmInter::DefinedLabel>>;
    Table m_tab;
};

class SymTable
{
public:
    SymTable();
    ~SymTable();
    int set_sym(const std::string &key,
                const std::shared_ptr<const AsmInter::Label> &sym);
    const std::shared_ptr<const AsmInter::Label> get_sym() const;
    const std::shared_ptr<const AsmInter::Stmt> solve(DefinedSymTable *st,
                                                      const Objects *objs,
                                                      int16_t global_offset,
                                                      size_t block_size) const;
private:
    using Table = std::unordered_map<std::string, std::shared_ptr<const AsmInter::Label>>;
    Table m_tab;
};

class ObjectFile
{
public:
    ObjectFile(const SymTable *st,
               const std::shared_ptr<const AsmInter::Stmt> &code);
    ~ObjectFile();
    const std::shared_ptr<const AsmInter::Label> find_sym() const;
    const std::shared_ptr<const AsmInter::Stmt> solve(DefinedSymTable *st,
                                                      const Objects *objs,
                                                      int16_t global_offset) const;
    const std::string compile(const Objects *objs) const;
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
    int add_file(const std::shared_ptr<const ObjectFile> &file);
    int delete_file(const std::shared_ptr<const AsmInter::Label> &sym);
    size_t n_files() const;
private:
    using Table = std::unordered_map<std::string, std::shared_ptr<const ObjectFile>>;
    Table m_files;
};

}

#endif // ASM_OBJECT_H_
