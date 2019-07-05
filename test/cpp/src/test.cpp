#include "vm/c/include/vm.h"
#include "vm/c/include/cmd.h"
#define __TEST__
#include "comp/cpp/include/lexer.h"
#include "comp/cpp/include/parser.h"
#include "asm/cpp/include/asmparser.h"
#include "function/cpp/include/exprcompilier.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#define DEF_ACCURANCY 1000000
#define TESTS 50
#define DEFAULT_TEST_FILE_PREF "test/tests/test"

struct vm_test_res
{
    char *prog;
    double res;
};

struct vm_test_res vm_tests[] =
{
    {
        .prog = "\x16\x66\x66\x66\x66\x66\x66\x24\x40\x16\x33\x33\x33\x33\x33\x33\x34\x40\x21\0",
        .res = 10
    }
};

static void
test_vm()
{
    printf("test virtual machine:\n");
    for (unsigned int i = 0; i < sizeof(vm_tests) / sizeof(struct vm_test_res); ++i)
    {
        double arr[1] = {0};
        double result = run(vm_tests[i].prog, 0, arr);
        if ((unsigned long) ((result - vm_tests[i].res) * DEF_ACCURANCY) == 0)
        {
            std::cout << "SUCCESS TEST № " << i << std::endl;
        }
        else
        {
            std::cout << "ERROR TEST № " << i << std::endl << "expected - " <<
                         vm_tests[i].res << std::endl << "recieved - " << result <<
                         std::endl << "HERE IS BYTECODE OF PROG:" << std::endl;
            for (int j = 0; vm_tests[i].prog[j] != '\0'; ++j)
            {
                printf("%x ", vm_tests[i].prog[j]);
            }
            printf("\n");
        }
    }
}

struct comp_lexer_test_res
{
    const char *str;
    std::string res;
};

struct comp_lexer_test_res comp_lex_tests[] =
{
    {
        .str = "hi, my name is Artemiy, i am 20.4 year old",
        .res = std::string("<hi | ID>\n<, | ,>\n<my | ID>\n<name | ID>\n<is | ID>\n"
                           "<Artemiy | ID>\n<, | ,>\n<i | ID>\n<am | ID>\n<20.4 | REAL>\n"
                           "<year | ID>\n<old | ID>\n")
    }
};

static void
test_comp_lexer()
{
    printf("test compilier lexer:\n");
    for (unsigned int i = 0; i < sizeof (comp_lex_tests) / sizeof (struct comp_lexer_test_res); ++i)
    {
        CompLexer::Lexer lex(comp_lex_tests[i].str);
        std::string result = lex.test();
        if (result == comp_lex_tests[i].res)
        {
            std::cout << "SUCCESS TEST № " << i << std::endl;
        }
        else
        {
            std::cout << "ERROR TEST № " << i << std::endl << "expected -" <<
                         std::endl << comp_lex_tests[i].res << std::endl <<
                         "recieved -" << std::endl << result << std::endl;
            std::cout << "SRC STRING:" << std::endl << comp_lex_tests[i].str <<
                         std::endl;
        }
    }
}

struct comp_parser_test_res
{
    const char *str;
    std::string res;
};

struct comp_parser_test_res comp_parser_tests[] =
{
    {
        .str = "test1(x,y)=2 + 2*sin(2/3 * 4) / x ^ y ^ y",
        .res = std::string("test1:\n\tFLD 2.000000\n\tFLD 2.000000\n\tFSUM\n")
    }
};

static
void
test_comp_parser()
{
    std::cout << "Test compilier parser:" << std::endl;
    for (unsigned int i = 0; i < sizeof (comp_parser_tests) / sizeof (struct comp_parser_test_res); ++i)
    {
        CompParser::Parser par(new CompLexer::Lexer(comp_parser_tests[i].str));
        std::cerr << "hmm" << std::endl;
        std::string result = par.parse()->gen();
        if (result == comp_parser_tests[i].res)
        {
            std::cout << "SUCCESS TEST № " << i << std::endl;
        }
        else
        {
            std::cout << "ERROR TEST № " << i << std::endl << "expected -" <<
                         std::endl << comp_parser_tests[i].res << std::endl <<
                         "recieved -" << std::endl << result << std::endl;
            std::cout << "SRC STRING:" << std::endl << comp_parser_tests[i].str <<
                         std::endl;
        }
    }
}

struct func_interface_test
{
    func_interface_test(const std::string &str,
                        const std::vector<double> &args) :
        m_str(str), m_args(args)
    {
    }
    func_interface_test(){}
    std::string m_str;
    std::vector<double> m_args;
};

static
void
get_func_interface_tests(std::vector<struct func_interface_test> &tests)
{
    std::string line("");
    for (int i = 0; i < TESTS; i++)
    {
        std::ifstream in(DEFAULT_TEST_FILE_PREF + std::to_string(i));

        struct func_interface_test tst;

        if (!in.is_open())
        {
            std::cerr << "unknown file: " << DEFAULT_TEST_FILE_PREF + std::to_string(i)
                      << std::endl;
            continue;
        }
        if (!getline(in, line))
        {
            std::cerr << "unexpected end of test file" << std::endl;
            continue;
        }
        tst.m_str = line;
        if (!getline(in, line))
        {
            std::cerr << "unexpected end of test file" << std::endl;
            continue;
        }
        std::istringstream iss(line);
        std::string number;
        while (iss >> number)
        {
            tst.m_args.push_back(std::stod(number));
        }
        tests.push_back(tst);
    }
}

static void
test_func_interface()
{
    std::cout << "test function interface" << std::endl;
    std::vector<struct func_interface_test> tests;

    get_func_interface_tests(tests);

    for (std::vector<double>::size_type i = 0; i < tests.size(); ++i)
    {
        std::cout << "test №" << i << std::endl;
        std::cout << "expr: " << tests[i].m_str << std::endl;
        std::cout << "args: ";
        for (const auto &n : tests[i].m_args)
        {
            std::cout << n << " ";
        }
        std::cout << std::endl;
        try
        {
            ExprCompilier::Function foo(tests[i].m_str.c_str());
            std::cout << "calc result: " << foo(tests[i].m_args.data()) << std::endl;
        }
        catch (std::exception &e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
}

int
main(int argc, char *argv[])
{
    test_comp_lexer();
    test_vm();
    test_comp_parser();
    test_func_interface();
    return 0;
}
