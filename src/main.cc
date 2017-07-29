#include <iostream>
#include <memory>

#include "peglib.hh"

#include "Parser.hh"
#include "ParseStruct.hh"

int main(int argc, const char** argv)
{
    if (argc < 2)
    {
        std::cout << "usage: calc [formula]" << std::endl;
        return 1;
    }

    Dlink::Parser parser;

    const char* expr = argv[1];
    Dlink::ExpressionPtr ast;

    if (parser.parse(expr, ast))
    {
        std::cout << "Parsing Succeed\n------------\n";
        std::cout << ast->tree_gen(0) << "\n";
        return 0;
    }
    else
    {
        std::cerr << "Syntax Error!\n";
        return -1;
    }
}
