#ifndef ASMINTER_H_
#define ASMINTER_H_

#include "comp/cpp/include/inter.h"

class Seq : public Stmt
{
public:
    Seq(Stmt *stmt1, Stmt *stmt2);
    virtual ~Seq();
    virtual std::string gen();
private:
    Stmt *stmt1;
    Stmt *stmt2;
};

class Cmd : public Stmt
{
public:
    Cmd(const std::string &opcode);
    virtual ~Cmd();
    virtual std::string gen();
private:
    std::string opcode;
};

#endif // ASMINTER_H_
