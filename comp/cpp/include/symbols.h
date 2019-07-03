#ifndef SYMBOLS_H
#define SYMBOLS_H

#include "cont/cpp/include/containers.h"
#include "lexer.h"
#include "inter.h"

#define TABLE_TYPE std::unordered_map<std::string, std::shared_ptr<const Inter::Id>>

namespace Symbols
{

class Env
{
public:
    Env();
    ~Env();
    void set(const std::shared_ptr<const CompLexer::Token> &w,
             const std::shared_ptr<const Inter::Id> &i);
    const std::shared_ptr<const Inter::Id> get(const std::shared_ptr<const CompLexer::Token> &w);
private:
    TABLE_TYPE m_tab;
protected:
    Env *m_prev;
};

}

#endif // SYMBOLS_H
