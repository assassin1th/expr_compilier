#ifndef ASMPARSER_H_
#define ASMPARSER_H_

#include "comp/cpp/include/parser.h"

class AsmParser : Parser
{
public:
    AsmParser(Lexer *lex);
    ~AsmParser();
private:

};

#endif // ASMPARSER_H_
