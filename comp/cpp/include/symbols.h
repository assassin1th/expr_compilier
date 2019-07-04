#ifndef SYMBOLS_H
#define SYMBOLS_H

#include "lexer.h"
#include "inter.h"

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
    using Table = std::unordered_map<std::string, std::shared_ptr<const Inter::Id>>;
    Table m_tab;
protected:
    Env *m_prev;
};

}

#endif // SYMBOLS_H
