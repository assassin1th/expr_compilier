#ifndef COMP_EXCEPT_H_
#define COMP_EXCEPT_H_

#include "lexer.h"

namespace CompExcept
{

class Exception
{
public:
    Exception();
    virtual ~Exception();
    virtual const char *what() const;
};

class UnexpectedSymbol : public Exception
{
public:
    UnexpectedSymbol(const std::string &src,
                const std::string::const_iterator &start,
                const std::string::const_iterator &end,
                int expected_sym);
    ~UnexpectedSymbol();
    const char * what() const;
private:
    std::string m_msg;
};
class SyntaxError : public Exception
{
public:
    SyntaxError(const std::string &src,
                const std::string::const_iterator &start,
                const std::string::const_iterator &end,
                const std::string &msg);
    ~SyntaxError();
    const char *what() const;
private:
    std::string m_msg;
};

class UndefinedVariable : public Exception
{
public:
    UndefinedVariable(const std::string &var_name);
    ~UndefinedVariable();
    const char *what() const;
private:
    std::string m_msg;
};

class AlreadyUsedVariable: public Exception
{
public:
    AlreadyUsedVariable(const std::string &var_name);
    ~AlreadyUsedVariable();
    const char *what() const;
private:
    std::string m_msg;
};

}

#endif // COMP_EXCEPT_H_
