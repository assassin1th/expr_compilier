#ifndef ASMPARSER_H_
#define ASMPARSER_H_

#ifdef __TEST__
#define __ASM_PARSER_TEST__
#endif // __TEST__

#include "comp/cpp/include/parser.h"
#include "asm/cpp/include/asminter.h"

namespace AsmParser {

class Parser
{
public:
    Parser(CompLexer::Lexer *lex);
    ~Parser();
    AsmInter::Obj *parse();
private:
    void move();
    void match(int tag);
    CompLexer::Token *m_look;
    CompLexer::Lexer *m_lex;
    AsmInter::LabelSeq *m_lbl_seq;
    Symbols::Env *m_env;
    int16_t m_offset;
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
