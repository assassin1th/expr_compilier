#include "function/cpp/include/function.h"
#include "comp/cpp/include/parser.h"
#include "asm/cpp/include/asmparser.h"
#include "asm/cpp/include/asmlexer.h"
#include "test/cpp/include/test.h"

using namespace Func;

Function::Function(const std::string &src)
{
    TEST_MSG(CompParser::Parser par(new CompLexer::Lexer(src)),
             "creating CompParser::Parser");
    std::shared_ptr<Inter::Stmt> stmt = par.parse();
    std::cerr << stmt->gen() << std::endl;
    TEST_MSG(AsmParser::Parser apar(new AsmLexer::Lexer(stmt->gen())),
             "creating AsmParser::Parser");
    std::shared_ptr<AsmInter::Obj> obj = apar.parse();
    std::cerr << obj->head_test() << std::endl;
    std::cerr << obj->cmd_test() << std::endl;
    std::cerr << obj->gen();
}

Function::~Function()
{
}

const std::string &
Function::code() const
{
    return m_byte_code;
}