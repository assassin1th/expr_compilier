#include "linker/cpp/include/linkerlexer.h"

using namespace LinkerLexer;

Lexer::Lexer(const std::string &src) :
    CompLexer::Lexer(src)
{
}

Lexer::~Lexer()
{
}

CompLexer::Token *
Lexer::scan()
{
    std::string buffer(*m_peek, sizeof (char));
    CompLexer::Token *tok = nullptr;

    if (std::isalpha(*m_peek))
    {
        do
        {
            readch();
            if (std::isalnum(*m_peek))
            {
                break;
            }

        } while (true);

        if ((tok = m_tab->get(buffer)) == nullptr)
        {
            m_tab->set((tok = new CompLexer::Word(Tag::ID, buffer)), buffer);
        }
        return tok;
    }

    if ((tok = m_tab->get(buffer)) == nullptr)
    {
        m_tab->set((tok = new CompLexer::Token(*m_peek)), buffer);
    }
    return tok;
}
