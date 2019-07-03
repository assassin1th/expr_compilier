#include "function/cpp/include/function.h"
#include "comp/cpp/include/parser.h"
#include "asm/cpp/include/asmparser.h"
#include "asm/cpp/include/asmlexer.h"
#include "test/cpp/include/test.h"

using namespace Func;

static AsmObject::Objects objs;

Function::Function(const std::string &src)
{
    TEST_MSG(CompParser::Parser par(new CompLexer::Lexer(src)),
             "creating CompParser::Parser");
    const std::shared_ptr<const Inter::Stmt> stmt = par.parse();
    std::cerr << stmt->gen() << std::endl;
    TEST_MSG(AsmParser::Parser apar(new AsmLexer::Lexer(stmt->gen())),
             "creating AsmParser::Parser");
    const std::shared_ptr<const AsmObject::ObjectFile> file = apar.parse();
    objs.add_file(file);
    m_byte_code = file->compile(&objs);
}

Function::~Function()
{
}

const std::string &
Function::code() const
{
    return m_byte_code;
}
