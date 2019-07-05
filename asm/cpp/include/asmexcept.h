#ifndef ASM_EXCEPT_H_
#define ASM_EXCEPT_H_

#include "comp/cpp/include/compexcept.h"

namespace AsmExcept
{

class RedefiningFunc : public CompExcept::Exception
{
public:
    RedefiningFunc(const std::string &sym_id);
    ~RedefiningFunc();
    const char *what() const;
private:
    std::string m_msg;
};

}

#endif // ASM_EXCEPT_H_
