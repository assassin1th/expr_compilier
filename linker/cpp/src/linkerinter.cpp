#include "linker/cpp/include/linkerinter.h"

using namespace LinkerInter;

static std::string tmp_string = "";

Code::Code()
{
}

Code::~Code()
{
}

const std::string &
Code::gen() const
{
    return tmp_string = "";
}

ByteCode::ByteCode(const std::string &bytes) :
    Code(), m_bytes(bytes)
{
}

ByteCode::~ByteCode()
{
}

const std::string &
ByteCode::gen() const
{
    return m_bytes;
}
