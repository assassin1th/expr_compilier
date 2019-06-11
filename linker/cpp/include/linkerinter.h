#ifndef LINKER_INTER_H_
#define LINKER_INTER_H_

#include <string>

namespace LinkerInter {

class Code
{
public:
    Code();
    virtual ~Code();
    virtual const std::string &gen() const;
};

}

#endif // LINKER_INTER_H_
