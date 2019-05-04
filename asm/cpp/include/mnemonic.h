#ifndef MNEMONIC_H_
#define MNEMONIC_H_

#include "vm/c/include/registers.h"
#include <string>
#include <stdint.h>
#include "aparser.h"

class Simplecmd
{
public:
    Simplecmd(uint8_t id);
    virtual ~Simplecmd();
    virtual std::string gen();
private:
    uint8_t id;
};

class Arithcmd : public Simplecmd
{
public:
    Arithcmd();
    virtual ~Arithcmd();
    virtual std::string gen();
private:
    uint8_t mode_flag;
};

class Compcmd : public Simplecmd
{
public:
    Compcmd();
    virtual ~Compcmd();
    virtual std::string gen();
private:
    Aparser *parser;
};

class Trigcmd : public Compcmd
{
public:
    Trigcmd();
    virtual ~Trigcmd();
    virtual std::string gen();
};

class Ldreg : public Compcmd
{
public:
    Ldreg();
    virtual ~Ldreg();
    virtual std::string gen();
};

class Callcmd : public Compcmd
{
public:
    Callcmd();
    virtual ~Callcmd();
    virtual std::string gen();
};

class Ldmem : public Compcmd
{
public:
    Ldmem();
    virtual ~Ldmem();
    virtual std::string gen();
};

class Ldreal : public Compcmd
{
public:
    Ldreal();
    virtual ~Ldreal();
    virtual std::string gen();
};

#endif // MNEMONIC_H_
