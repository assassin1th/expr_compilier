#include "asm/cpp/include/asmexcept.h"

using namespace AsmExcept;

RedefiningFunc::RedefiningFunc(const std::string &sym_id) :
    m_msg("Redefining function expression - " + sym_id)
{
}

RedefiningFunc::~RedefiningFunc()
{
}

const char *
RedefiningFunc::what() const
{
    return m_msg.c_str();
}
