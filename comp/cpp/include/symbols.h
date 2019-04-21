#ifndef SYMBOLS_H
#define SYMBOLS_H

#include "cont/cpp/include/hashtable.h"
#include "lexer.h"
#include "inter.h"

class Env
{
public:
    Env();
    ~Env();
    void set(Token *w, Id *i);
    Id *get(Token *w);
private:
    Cont<Id> *tab;
protected:
    Env *prev;
};

#endif // SYMBOLS_H
