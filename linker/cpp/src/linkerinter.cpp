#include "linker/cpp/include/linkerinter.h"

using namespace LinkerInter;

static const std::string null_string= "";

Code::Code()
{
}

Code::~Code()
{
}

const std::string &
Code::gen() const
{
    return null_string;
}
