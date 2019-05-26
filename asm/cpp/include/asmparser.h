#ifndef ASMPARSER_H_
#define ASMPARSER_H_

#include "comp/cpp/include/parser.h"

namespace AsmParser {

class Parser
{
public:
    Parser(CompLexer::Lexer *lex);
    ~Parser();
    Inter::Stmt *parse();
private:
    void move();
    void match(int tag);
    CompLexer::Token *m_look;
    CompLexer::Lexer *m_lex;
    Inter::Stmt *stmts();
    Inter::Stmt *stmt();
    Inter::Stmt *cmd();
    Inter::Stmt *label();
};

}
#endif // ASMPARSER_H_
