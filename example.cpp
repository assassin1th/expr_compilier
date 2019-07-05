#include <exprcompilier.h>
#include <iostream>
#include <vector>
#include <sstream>

std::vector<double> get_args()
{
    std::vector<double> args;
    std::string arg_str;
    std::getline(std::cin, arg_str);
    std::stringstream in(arg_str);

    std::string arg;
    while (in >> arg)
    {
        args.push_back(std::stod(arg));
    }
    return args;
}

int main(int argc, char *argv[])
{
    std::cout << "it's an example of use my function interface" << std::endl << std::endl;
    std::cout << "supported math operations:" << std::endl;
    std::cout << "'+' - sum operands" << std::endl << "'-' - substract operands" << std::endl
              << "'/' - divide operands" << std::endl << "'*' - multiply operands" << std::endl
              << "'^' - power operands" << std::endl << "log(x, y) - logarith of y by base x"
              << std::endl << "sin(x)" << std::endl << "cos(x)" << std::endl <<"tan(x)"
              << std::endl << "ctan(x)" << std::endl << "asin(x)" << std::endl
              << "acos(x)" << std::endl << "atan(x)" << std::endl << "actan(x)"
              << std::endl << std::endl;
    std::string expr;
    std::cout << "enter your function" << std::endl;
    std::cout << "${FOO_NAME} ( ${VARIABLES} ) = ${YOUR EXPRESSION}" << std::endl;
    while (std::getline(std::cin, expr))
    {

        try
        {
            // create function from expr string
            ExprCompilier::Function func(expr);

            std::cout << "enter arguments" << std::endl;
            std::vector<double> args = get_args();

            // run function with array of (double) arguments
            std::cout << "result:" << func(args.data()) << std::endl;
        }
        catch (CompExcept::Exception &e)
        {
            std::cerr << e.what() << std::endl;
        }
        std::cout << "enter your function:" << std::endl;
        std::cout << "${FOO_NAME} ( ${VARIABLES} ) = ${YOUR EXPRESSION}" << std::endl;
    }

    return 0;
}
