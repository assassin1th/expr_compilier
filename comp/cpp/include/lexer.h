#ifndef LEXER_H_
#define LEXER_H_

#ifdef __TEST__
#define __COMP_LEXER_TEST__
#endif // __TEST__

#include <string>
#include <memory>
#include <unordered_map>
#include "cont/cpp/include/containers.h"

#define TABLE_TYPE std::unordered_map<std::string, std::shared_ptr<Token>>

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
    virtual const std::string &val() const;
private:
    int m_tag;
protected:
    static std::string tmp_string;
};

class Word : public Token
{
public:
    Word(int tag, const std::string &lexeme);
    virtual ~Word();
    virtual const std::string &val() const;
private:
    std::string m_lexeme;
};

class Real : public Token
{
public:
    Real(const std::string &num);
    ~Real();
    virtual const std::string &val() const;
private:
    std::string m_num;
};

class Lexer
{
public:
    Lexer(const std::string &src);
    virtual ~Lexer();
    virtual std::shared_ptr<Token> scan();
#ifdef __COMP_LEXER_TEST__
    const std::string test();
#endif // LEXER_TEST_
    void reserve(Token *tok, const std::string &key);
private:
    const std::string m_src_str;
protected:
    void readch();
    bool readch(int c);
    std::string::const_iterator m_peek;
    TABLE_TYPE m_tab;

};

}
#endif // LEXER_H_
