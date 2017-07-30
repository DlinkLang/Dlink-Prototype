#include <iostream>
#include <memory>

#include "peglib.hh"

#include "Parser.hh"
#include "ParseStruct.hh"
#include "CodeGen.hh"

int main(int argc, const char** argv)
{
    if (argc < 2)
    {
        std::cout << "usage: Dlink [expression]" << std::endl;
        return 1;
    }

    Dlink::Parser parser;

    const char* expr = argv[1];
    Dlink::ExpressionPtr ast;

    if (parser.parse(expr, ast))
    {
        std::cout << "Parsing Sucessed\n\n";
        std::cout << "<Generated Parse Tree>\n";
        std::cout << ast->tree_gen(0) << "\n\n";
 
        std::cout << "<Generated LLVM IR>\n";
        ast->code_gen(); 
        Dlink::LLVM::module->dump();

        return 0;
    }
    else
    {
        std::cerr << "Syntax Error!\n";
        return -1;
    }
}
