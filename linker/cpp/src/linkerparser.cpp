#include "linker/cpp/include/linkerparser.h"
#include <iostream>

using namespace LinkerParser;

Parser::Parser(LinkerLexer::Lexer *lex) :
    m_lex(lex), env(new LinkerSymbols::Env())
{
    m_lex->reserve(new CompLexer::Word(LinkerLexer::Tag::SYMTAB, "SYMTAB"), "SYMTAB");
    m_lex->reserve(new CompLexer::Word(LinkerLexer::Tag::DEFFLAG, "D"), "D");
    m_lex->reserve(new CompLexer::Word(LinkerLexer::Tag::UNDEFFLAG, "U"), "U");
    move();
}

Parser::~Parser()
{
    delete m_lex;
}

void
Parser::move()
{
    m_look = m_lex->scan();
}

void
Parser::match(int tag)
{

    std::cerr << "hello1" << std::endl;
    if (m_look->tag() != tag)
    {
        std::cerr << __FILE__ << ": " << __LINE__ << ":\n" <<
                     "unknown symbol" << std::endl;
    }
    move();
}

const LinkerObject::ObjectFile *
Parser::parse()
{
    const LinkerObject::ObjectHeader *objh = header();
    match('\n');
    return new LinkerObject::ObjectFile(objh, code());
}

const LinkerObject::SymTable *
Parser::symtab()
{
    LinkerObject::SymTable *st = new LinkerObject::SymTable();
    while (m_look->tag() == '\n')
    {
        move();
        if (m_look->tag() == '\n')
        {
            break;
        }
        else
        {
            env->set(m_look, new LinkerInter::Sym(m_look->val()));
            st->set_sym(symlink());
        }

    }
    return st;
}

const LinkerSymbols::SymLink *
Parser::symlink()
{
    CompLexer::Token *id = m_look;
    match(LinkerLexer::Tag::ID);

    std::cerr << "hello" << std::endl;
    if (m_look->tag() == LinkerLexer::Tag::DEFFLAG)
    {
        move();
        CompLexer::Token *num = m_look;
        if (num == nullptr)
        {
            std::cerr << "nullptr" << std::endl;
        }
        match(LinkerLexer::Tag::NUMBER);
        return new LinkerSymbols::DefinedSymLink(id->val(), std::stoi(num->val()));
    }
    else
    {
        move();
        return new LinkerSymbols::SymLink(id->val());
    }
}

const LinkerObject::ObjectHeader *
Parser::header()
{
    if (m_look->tag() == LinkerLexer::Tag::SYMTAB)
    {
        move();
        match(':');
        return new LinkerObject::ObjectHeader(symtab());
    }
    else
    {
        return nullptr;
    }
}

const LinkerInter::Code *
Parser::code()
{
    const LinkerInter::Code *code = symcode();
    while (m_look->tag() != '\0')
    {
        std::cerr << m_look->tag();
        code = new LinkerInter::CodeSeq(code, symcode());
    }
    return code;
}

const LinkerInter::Code *
Parser::symcode()
{
    const LinkerInter::Code *code = objcode();
    if (env->get(m_look))
    {
        code = new LinkerInter::SymCode(code, new LinkerInter::Sym(m_look->val()));
        move();
    }
    return code;
}

const LinkerInter::Code *
Parser::objcode()
{
    std::string buf;
    do
    {
        buf += m_look->val();
        move();
    }
    while (m_look->tag() != LinkerLexer::Tag::ID);
    return new LinkerInter::ByteCode(buf);
}





































