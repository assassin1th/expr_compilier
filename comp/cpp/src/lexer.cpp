#include "comp/cpp/include/lexer.h"
#include <ctype.h>
#include <iostream>

using namespace CompLexer;

Token::Token(int tag) :
    m_tag(tag)
{
}

Token::~Token()
{
}

int
Token::tag() const
{
    return m_tag;
}

const std::string
Token::val() const
{
    return std::string(sizeof(char), m_tag);
}

Word::Word(int tag, const std::string &lexeme) :
    Token(tag), m_lexeme(lexeme)
{
}

Word::~Word()
{
}

const std::string
Word::val() const
{
    return m_lexeme;
}

Real::Real(const std::string &num) :
    Token(Tag::REAL), m_num(num)
{
}

Real::~Real()
{
}

const std::string
Real::val() const
{
    return m_num;
}

Lexer::Lexer(const std::string &src) :
    m_src_str(src), m_peek(m_src_str.begin())
{
    m_tab = new Containers::Table<Token>();
}

Lexer::~Lexer()
{
    delete m_tab;
}

void
Lexer::readch()
{
    m_peek++;
}

bool
Lexer::readch(int c)
{
    readch();
    if (*m_peek == c) {
        return true;
    }
    return false;
}

void
Lexer::reserve(Token *tok, const std::string &key)
{
    m_tab->set(tok, key);
}

Token *
Lexer::scan()
{
    for (;;readch())
    {
        if (isspace(*m_peek))
        {
            continue;
        } else {
            break;
        }
    }
    std::string buf = std::string();
    if (isalpha(*m_peek))
    {
        do {
            buf.push_back(*m_peek);
            readch();
        } while (isalnum(*m_peek));

        Token *w = m_tab->get(buf);

        if (w != nullptr) {
            return w;
        }
        m_tab->set(w = new Word(Tag::ID, buf), buf);
        return w;
    } else if (isdigit(*m_peek)) {
        do {
            buf.push_back(*m_peek);
            readch();
        } while (isdigit(*m_peek));

        if (*m_peek == '.')
        {
            do {
                buf.push_back(*m_peek);
                readch();
            } while (isdigit(*m_peek));
        }
        Token *r = m_tab->get(buf);

        if (r != nullptr)
        {
            return r;
        }
        m_tab->set(r = new Real(buf), buf);
        return r;
    }
    buf.push_back(*m_peek);
    Token *t = m_tab->get(buf);
    if (t != nullptr)
    {
        readch();
        return t;
    }
    m_tab->set(t = new Token(*m_peek), buf);

    readch();
    return t;
}

#ifdef LEXER_TEST_
const std::string
Lexer::test()
{
    std::string result;
    for (Token *t = scan(); t->tag() != '\0'; t = scan())
    {
        result += "<" + t->val() + " | ";
        switch (t->tag())
        {
            case Tag::ID:
                result.append(std::string("ID"));
                break;
            case Tag::REAL:
                result.append(std::string("REAL"));
                break;
            default:
                result.append(std::string(sizeof(char), t->tag()));
                break;
        }
        result += ">\n";
    }
    return result;
}
#endif // LEXER_TEST_
