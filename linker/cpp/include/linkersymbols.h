#ifndef LIKER_INTER_H_
#define LIKER_INTER_H_
#include "linkerinter.h"
#include "cont/cpp/include/containers.h"

namespace LinkerSymbols {

class SymLink : public LinkerInter::Sym
{
public:
    SymLink(const CompLexer::Token *tok,
            LinkerInter::SymTag tag = LinkerInter::SymTag::UNDEFINED);
    virtual ~SymLink();
private:
    const LinkerInter::SymTag m_tag;
};

class DefinedSymLink : public SymLink
{
public:
    DefinedSymLink(const CompLexer::Token *tok, int16_t offset);
    virtual ~DefinedSymLink();
    virtual const std::string gen() const;
private:
    const int16_t m_offset;
};


class Env
{
public:
    Env();
    ~Env();
    const SymLink *get(const CompLexer::Token *tok) const;
    void set(const CompLexer::Token *tok,
                       const SymLink *sym);
private:
    Containers::Table<SymLink> *m_tab;
};


}

#endif // LIKER_INTER_H_
