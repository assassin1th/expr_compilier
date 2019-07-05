#include "comp/cpp/include/compexcept.h"

using namespace CompExcept;

Exception::Exception()
{
}

Exception::~Exception()
{
}

const char *
Exception::what() const
{
    return "CompExecp::Exception";
}

UnexpectedSymbol::UnexpectedSymbol(const std::string &src,
                         const std::string::const_iterator &start,
                         const std::string::const_iterator &end,
                         int expected_sym):
    m_msg(src)
{
    m_msg += "\n";
    for (std::string::const_iterator it = src.begin(); it < start; ++it)
    {
        m_msg += "~";
    }
    for (std::string::const_iterator it = start; it < end; ++it)
    {
        m_msg += "^";
    }
    for (std::string::const_iterator it = end; it < src.end(); ++it)
    {
        m_msg += "~";
    }
    m_msg += "\nexpected ";
    switch (expected_sym)
    {
        case CompLexer::Tag::ID:
            m_msg += "ID";
            break;
        case CompLexer::Tag::COS:
            m_msg += "COS";
            break;
        case CompLexer::Tag::LOG:
            m_msg += "LOG";
            break;
        case CompLexer::Tag::SIN:
            m_msg += "SIN";
            break;
        case CompLexer::Tag::TAN:
            m_msg += "TAN";
            break;
        case CompLexer::Tag::CTAN:
            m_msg += "CTAN";
            break;
        case CompLexer::Tag::ACOS:
            m_msg += "ACOS";
            break;
        case CompLexer::Tag::ASIN:
            m_msg += "ASIN";
            break;
        case CompLexer::Tag::ATAN:
            m_msg += "ATAN";
            break;
        case CompLexer::Tag::ACTAN:
            m_msg += "ACTAN";
            break;
        case CompLexer::Tag::REAL:
            m_msg += "REAL";
            break;
        default:
            m_msg += "'" + std::string(sizeof (char), expected_sym) + "'";
            break;
    }
}

UnexpectedSymbol::~UnexpectedSymbol()
{
}

const char *
UnexpectedSymbol::what() const
{
    return m_msg.c_str();
}

SyntaxError::SyntaxError(const std::string &src,
                         const std::string::const_iterator &start,
                         const std::string::const_iterator &end,
                         const std::string &msg) :
    m_msg(src)
{
    if (m_msg.back() != '\n')
    {
        m_msg.push_back('\n');
    }
    for (std::string::const_iterator it = src.begin(); it < start; ++it)
    {
        m_msg.push_back('~');
    }
    for (std::string::const_iterator it = start; it < end; ++it)
    {
        m_msg.push_back('^');
    }
    for (std::string::const_iterator it = end; it < src.end(); ++it)
    {
        m_msg.push_back('~');
    }
    m_msg += "\n" + msg;
}

SyntaxError::~SyntaxError()
{
}

const char *
SyntaxError::what() const
{
    return m_msg.c_str();
}

UndefinedVariable::UndefinedVariable(const std::string &var_name) :
    m_msg("Undefined variable - " + var_name)
{
}

UndefinedVariable::~UndefinedVariable()
{
}

const char *
UndefinedVariable::what() const
{
    return m_msg.c_str();
}

AlreadyUsedVariable::AlreadyUsedVariable(const std::string &var_name) :
    m_msg("Already used variable " + var_name)
{
}

AlreadyUsedVariable::~AlreadyUsedVariable()
{
}

const char *
AlreadyUsedVariable::what() const
{
    return m_msg.c_str();
}
