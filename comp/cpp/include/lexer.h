#ifndef LEXER_H_
#define LEXER_H_

#include <string>
#include "cont/cpp/include/hashtable.h"


enum Tag {
    ID = 256,
    REAL
};

class Token {
public:
    Token(int tag);
    virtual ~Token();
    int tag();
    virtual std::string val();
private:
    int _tag;
};

class Word : public Token {
public:
    Word(int tag, const std::string lexeme);
    virtual ~Word();
    virtual std::string val();
private:
    std::string _lexeme;
};

class Real : public Token {
public:
    Real(const std::string num);
    ~Real();
    virtual std::string val();
private:
    std::string _num;
};

class Lexer
{
public:
    Lexer(const std::string src);
    ~Lexer();
    Token *scan();
private:
    void readch();
    void reserve();
    bool readch(int c);
    Table<Token> *_tab;
    std::string::const_iterator _str;
    char _peek;
};

#endif // LEXER_H_
