#ifndef FUNCTION_H_
#define FUNCTION_H_

#include <string>
#include <memory>

namespace CompExcept
{
class Exception
{
public:
    Exception();
    ~Exception();
    const char *what() const;
};
}

namespace AsmInter
{
    class Label;
}

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
    std::shared_ptr<const AsmInter::Label> m_sym;
};

}

#endif // FUNCTION_H_
