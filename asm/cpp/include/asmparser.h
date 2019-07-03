#ifndef ASMPARSER_H_
#define ASMPARSER_H_

#ifdef __TEST__
#define __ASM_PARSER_TEST__
#endif // __TEST__

#include "comp/cpp/include/parser.h"
#include "asm/cpp/include/asminter.h"
#include "asm/cpp/include/asmobject.h"

namespace AsmParser {

class Parser
{
public:
    Parser(CompLexer::Lexer *lex);
    ~Parser();
    const std::shared_ptr<const AsmObject::ObjectFile> parse();
private:
    void move();
    void match(int tag);
    std::shared_ptr<const CompLexer::Token> m_look;
    CompLexer::Lexer *m_lex;
    AsmObject::SymTable *m_st;
    int16_t m_offset;
    const std::shared_ptr<const AsmInter::Stmt> stmts();
    const std::shared_ptr<const AsmInter::Stmt> stmt();
    const std::shared_ptr<const AsmInter::Stmt> label();
    const std::shared_ptr<const AsmInter::Stmt> arith_cmd();
    const std::shared_ptr<const AsmInter::Stmt> trig_cmd();
    const std::shared_ptr<const AsmInter::Stmt> ld_cmd();
    const std::shared_ptr<const AsmInter::Stmt> call_cmd();
    const std::shared_ptr<const AsmInter::Stmt> cmd();
    const std::shared_ptr<const AsmInter::Reg> reg();
    const std::shared_ptr<const AsmInter::Offset> offset();
    const std::shared_ptr<const AsmInter::Expr> expr();
};

}
#endif // ASMPARSER_H_
