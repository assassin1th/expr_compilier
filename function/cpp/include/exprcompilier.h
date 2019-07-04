#ifndef FUNCTION_H_
#define FUNCTION_H_

#include <string>

namespace ExprCompilier
{

class Function
{
public:
    Function(const std::string &src);
    ~Function();
    const std::string &code() const;
    double operator()(double *args);
private:
    std::string m_byte_code;
    size_t m_n_args;
};

}

#endif // FUNCTION_H_
