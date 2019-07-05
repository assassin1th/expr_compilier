#include "function/cpp/include/exprcompilier.h"
#include "comp/cpp/include/parser.h"
#include "asm/cpp/include/asmparser.h"
#include "asm/cpp/include/asmlexer.h"
#include "test/cpp/include/test.h"
#include "vm/c/include/vm.h"
using namespace ExprCompilier;

static AsmObject::Objects objs;

Function::Function(const std::string &src) :
    m_byte_code("")
{
    TEST_MSG(CompParser::Parser par(new CompLexer::Lexer(src)),
             "creating CompParser::Parser");
    const std::shared_ptr<const Inter::FuncDecl> decl = par.parse();
#ifdef __DEBUG__
    std::cerr << decl->gen() << std::endl;
#endif // __DEBUG
    m_n_args = decl->n_arg();
    TEST_MSG(AsmParser::Parser apar(new AsmLexer::Lexer(decl->gen())),
             "creating AsmParser::Parser");
    TEST_MSG(const std::shared_ptr<const AsmObject::ObjectFile> file = apar.parse(),
             "creating AsmObject::ObjectFile");
    m_sym = file->find_sym();
    TEST_MSG(objs.add_file(file),
             "add file to object files collection");
    TEST_MSG(m_byte_code = file->compile(&objs),
             "compile file");
}

Function::~Function()
{
    objs.delete_file(m_sym);
}

const std::string &
Function::code() const
{
    return m_byte_code;
}

double
Function::operator()(double *args)
{
    return run(m_byte_code.c_str(), m_n_args, args);
}
