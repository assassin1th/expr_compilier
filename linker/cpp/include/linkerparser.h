#ifndef LINKER_PARSER_H_
#define LINKER_PARSER_H_

#include "linkerobject.h"
#include "linkerinter.h"
#include "linkerlexer.h"

namespace LinkerParser
{

class Parser
{
public:
    Parser(LinkerLexer::Lexer *lex);
    ~Parser();
    const LinkerObject::ObjectFile *parse();
private:
    CompLexer::Token *m_look;
    LinkerLexer::Lexer *m_lex;
    LinkerSymbols::Env *env;
    void move();
    void match(int ch);
    const LinkerObject::SymTable *symtab();
    const LinkerObject::ObjectHeader *header();
    const LinkerInter::Code *code();
    const LinkerSymbols::SymLink *symlink();
    const LinkerInter::Sym *sym();
    const LinkerInter::Code *symcode();
    const LinkerInter::Code *objcode();

};

}

#endif // LINKER_PARSER_H_
