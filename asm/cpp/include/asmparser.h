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
    std::shared_ptr<AsmInter::Obj> parse();
private:
    void move();
    void match(int tag);
    std::shared_ptr<CompLexer::Token> m_look;
    CompLexer::Lexer *m_lex;
    std::shared_ptr<AsmInter::LabelSeq> m_lbl_seq;
    int16_t m_offset;
    std::shared_ptr<AsmInter::Stmt> stmts();
    std::shared_ptr<AsmInter::Stmt> stmt();
    std::shared_ptr<AsmInter::Stmt> label();
    std::shared_ptr<AsmInter::Stmt> arith_cmd();
    std::shared_ptr<AsmInter::Stmt> trig_cmd();
    std::shared_ptr<AsmInter::Stmt> ld_cmd();
    std::shared_ptr<AsmInter::Stmt> call_cmd();
    std::shared_ptr<AsmInter::Stmt> cmd();
    std::shared_ptr<AsmInter::Reg> reg();
    std::shared_ptr<AsmInter::Offset> offset();
    std::shared_ptr<AsmInter::Expr> expr();
};

}
#endif // ASMPARSER_H_
