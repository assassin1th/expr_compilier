#ifndef SYMBOLS_H
#define SYMBOLS_H

#include "cont/cpp/include/containers.h"
#include "lexer.h"
#include "inter.h"

namespace Symbols
{

class Env
{
public:
    Env();
    ~Env();
    void set(CompLexer::Token *w, Inter::Id *i);
    Inter::Id *get(CompLexer::Token *w);
private:
    Containers::Cont<Inter::Id> *m_tab;
protected:
    Env *m_prev;
};

}

#endif // SYMBOLS_H
