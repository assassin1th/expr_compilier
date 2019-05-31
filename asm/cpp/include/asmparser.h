#ifndef ASMPARSER_H_
#define ASMPARSER_H_

#include "comp/cpp/include/parser.h"
#include "asm/cpp/include/asminter.h"
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
    AsmInter::LabelSeq *m_lbl_seq;
    Symbols::Env *m_env;
    Inter::Stmt *stmts();
    Inter::Stmt *stmt();
    Inter::Stmt *label();
    Inter::Stmt *arith_cmd();
    Inter::Stmt *trig_cmd();
    Inter::Stmt *ld_cmd();
    Inter::Stmt *call_cmd();
    Inter::Stmt *cmd();
    AsmInter::Reg *reg();
    AsmInter::Offset *offset();
    AsmInter::Expr *expr();
};

}
#endif // ASMPARSER_H_
