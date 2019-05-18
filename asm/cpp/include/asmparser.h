#ifndef ASMPARSER_H_
#define ASMPARSER_H_

#include "comp/cpp/include/parser.h"

class AsmParser : Parser
{
public:
    AsmParser(Lexer *lex);
    ~AsmParser();
private:
    Stmt *stmts();
    Stmt *stmt();
};

#endif // ASMPARSER_H_
