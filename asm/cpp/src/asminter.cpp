#include "asm/cpp/include/asminter.h"
#include <string>

Seq::Seq(Stmt *stmt1, Stmt *stmt2) :
    Stmt(), stmt1(stmt1), stmt2(stmt2)
{
}

Seq::~Seq()
{
}

std::string
Seq::gen()
{
    return stmt1->gen() + stmt2->gen();
}

Cmd::Cmd(const std::string &opcode) :
    Stmt(), opcode(opcode)
{
}

Cmd::~Cmd()
{
}

std::string
Cmd::gen()
{
    return opcode;
}
