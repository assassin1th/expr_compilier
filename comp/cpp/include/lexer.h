#ifndef LEXER_H_
#define LEXER_H_

#include <string>
#include "cont/cpp/include/containers.h"

namespace CompLexer
{

enum Tag
{
    ID = 256, SIN, COS, TAN, CTAN, LOG,
    ASIN, ACOS, ATAN, ACTAN, REAL
};

class Token
{
public:
    Token(int tag);
    virtual ~Token();
    int tag() const;
    virtual const std::string val() const;
private:
    int m_tag;
};

class Word : public Token
{
public:
    Word(int tag, const std::string &lexeme);
    virtual ~Word();
    virtual const std::string val() const;
private:
    std::string m_lexeme;
};

class Real : public Token
{
public:
    Real(const std::string &num);
    ~Real();
    virtual const std::string val() const;
private:
    std::string m_num;
};

class Lexer
{
public:
    Lexer(const std::string &src);
    ~Lexer();
    Token *scan();
#ifdef LEXER_TEST_
    const std::string test();
#endif // LEXER_TEST_
    void reserve(Token *tok, const std::string &key);
private:
    void readch();
    bool readch(int c);
    const std::string m_src_str;
    std::string::const_iterator m_peek;
    Containers::Table<Token> *m_tab;
};

}
#endif // LEXER_H_
