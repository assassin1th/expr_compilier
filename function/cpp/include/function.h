#ifndef FUNCTION_H_
#define FUNCTION_H_

#include <string>

namespace Func {

class Function
{
public:
    Function(const std::string &src);
    ~Function();
    const std::string &code() const;
private:
    std::string m_byte_code;
};

}

#endif // FUNCTION_H_
