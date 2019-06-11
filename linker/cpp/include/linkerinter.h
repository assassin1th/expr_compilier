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
protected:
    static std::string tmp_string;
};

class ByteCode : public Code
{
public:
    ByteCode(const std::string &bytes);
    virtual ~ByteCode();
    virtual const std::string &gen() const;
private:
    const std::string m_bytes;
};

}


#endif // LINKER_INTER_H_
