#ifndef COMP_EXCEPT_H_
#define COMP_EXCEPT_H_

#include <exception>
#include "lexer.h"

namespace CompExept
{

class UnexpectedSymbol : public std::exception
{
public:
    UnexpectedSymbol(const std::string &src,
                const std::string::const_iterator &start,
                const std::string::const_iterator &end,
                int expected_sym);
    ~UnexpectedSymbol();
    const char * what();
private:
    std::string m_msg;
};

}

#endif // COMP_EXCEPT_H_
