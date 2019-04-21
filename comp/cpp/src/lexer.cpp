#include "comp/cpp/include/lexer.h"
#include <ctype.h>
#include <iostream>
Token::Token(int tag) :
    _tag(tag)
{
}

Token::~Token()
{
}

int Token::tag()
{
    return _tag;
}

std::string Token::val()
{
    return std::string(sizeof(char), _tag);
}

Word::Word(int tag, const std::string lexeme) :
    Token(tag), _lexeme(lexeme)
{
}

Word::~Word()
{
}

std::string Word::val()
{
    return _lexeme;
}

Real::Real(const std::string num) :
    Token(Tag::REAL), _num(num)
{
}

Real::~Real()
{
}

std::string Real::val()
{
    return _num;
}

Lexer::Lexer(const std::string src) :
    _peek(' ')
{
    _tab = new Table<Token>();
    _str = src.begin();
}

void Lexer::readch()
{
    std::cerr << *_str << std::endl;
    _peek = *_str++;
}

bool Lexer::readch(int c)
{
    readch();
    if (_peek == c) {
        return true;
    }
    return false;
}

Token *Lexer::scan()
{
    std::cout << "ku1" << std::endl;
    for (;;readch())
    {
        if (isspace(_peek))
        {
            continue;
        } else {
            break;
        }
    }
    if (isalpha(_peek))
    {
        std::string buf = std::string();

        do {
            buf.push_back(_peek);
            readch();
        } while (isalnum(_peek));

        Token *w = _tab->get(buf);

        if (w != nullptr) {
            return w;
        }

        std::cout << buf << std::endl;
        _tab->set(w = new Word(Tag::ID, buf), buf);
        return w;
    } else if (isdigit(_peek)) {
        std::string buf = std::string();

        do {
            buf.push_back(_peek);
            readch();
        } while (isdigit(_peek));

        if (_peek == '.')
        {
            do {
                buf.push_back(_peek);
                readch();
            } while (isdigit(_peek));
        }

        std::cout << "ku2" << std::endl;
        std::cout << buf << std::endl;
        Token *r = _tab->get(buf);

        if (r != nullptr)
        {
            std::cout << r->val() << std::endl;
            return r;
        }
        std::cout << "ku2" << std::endl;

        _tab->set(r = new Real(buf), buf);

        std::cout << "ku2" << std::endl;
            return r;
    }
    std::cout << "ku2" << std::endl;
    Token *t = new Token(_peek);
    _peek = ' ';
    return t;
}
