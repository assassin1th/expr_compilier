#include "comp/cpp/include/lexer.h"
#include "test/cpp/include/test.h"
#include <ctype.h>
#include <iostream>

using namespace CompLexer;

std::string Token::tmp_string = "";

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

const std::string &
Token::val() const
{
    return tmp_string = std::string(sizeof(char), m_tag);
}

Word::Word(int tag, const std::string &lexeme) :
    Token(tag), m_lexeme(lexeme)
{
}

Word::~Word()
{
}

const std::string &
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

const std::string &
Real::val() const
{
    return m_num;
}

Lexer::Lexer(const std::string &src) :
    m_src_str(src), m_peek(m_src_str.begin())
{

}

Lexer::~Lexer()
{
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
    m_tab[key] = std::shared_ptr<Token> (tok);
}

std::shared_ptr<const Token>
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

        TABLE_TYPE::iterator it = m_tab.find(buf);

        if (it == m_tab.end()) {
            const std::shared_ptr<const Token> w (new Word(Tag::ID, buf));
            m_tab[buf] = w;
            return w;
        }

        return it->second;
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
        TABLE_TYPE::iterator it = m_tab.find(buf);
        if (it == m_tab.end())
        {
            const std::shared_ptr<const Token> r (new Real(buf));
            m_tab[buf] = r;
            return r;
        }
        return it->second;
    }
    buf.push_back(*m_peek);
    TABLE_TYPE::iterator it = m_tab.find(buf);
    if (it == m_tab.end())
    {
        const std::shared_ptr<const Token> t(new Token(*m_peek));
        m_tab[buf] = t;
        readch();
        return t;
    }
    readch();
    return it->second;
}

#ifdef __COMP_LEXER_TEST__
const std::string
Lexer::test()
{
    std::string result;
    for (std::shared_ptr<const Token> t = scan(); t->tag() != '\0'; t = scan())
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
