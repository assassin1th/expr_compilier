#include "linker/cpp/include/linkerlexer.h"

using namespace LinkerLexer;

Number::Number(const std::string &num) :
    Token(Tag::NUMBER), m_num(num)
{
}

Number::~Number()
{
}

const std::string &
Number::val() const
{
    return m_num;
}

Lexer::Lexer(const std::string &src) :
    CompLexer::Lexer(src)
{
}

Lexer::~Lexer()
{
}

Bytes::Bytes(const std::string &bytes) :
    CompLexer::Word(Tag::BYTES, bytes)
{
}

Bytes::~Bytes()
{
}

CompLexer::Token *
Lexer::scan()
{
    std::string buffer;
    for (;; readch())
    {
        if (*m_peek != '\n' && isspace(*m_peek))
        {
            continue;
        }
        else
        {
            break;
        }
    }
    if (isalpha(*m_peek))
    {
        do
        {
            buffer.push_back(*m_peek);
            readch();
        }
        while (isalnum(*m_peek));

        CompLexer::Token *tok = m_tab->get(buffer);
        if (tok == nullptr)
        {
            m_tab->set((tok = new CompLexer::Word(Tag::ID, buffer)),
                       buffer);
        }

        return tok;
    }
    else if (isdigit(*m_peek))
    {
        do
        {
            buffer.push_back(*m_peek);
            readch();
        }
        while (isdigit(*m_peek));

        CompLexer::Token *tok = m_tab->get(buffer);
        if (tok == nullptr)
        {
            m_tab->set((tok = new Number(buffer)), buffer);
        }

        return tok;
    }
    else
    {
        buffer.push_back(*m_peek);
        CompLexer::Token *tok = m_tab->get(buffer);
        if (tok == nullptr)
        {
            m_tab->set((tok = new CompLexer::Token(*m_peek)), buffer);
        }
        readch();
        return tok;
    }
}
