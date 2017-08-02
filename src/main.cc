#include <iostream>
#include <memory>
#include <sstream>

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

	std::string input = argv[1];

	for (int i = 2; i < argc; ++i)
	{
		input += ' ' + std::string(argv[i]);
	}

    auto on_error = [&](std::size_t ln, std::size_t col, const std::string& msg)
    {
        std::istringstream input_stream(input);

        std::string line;
       
        std::size_t current_ln = 1;

        while(std::getline(input_stream, line) && current_ln++ != ln);
        
        std::cerr << "Error occurred!\n";
        std::cerr << "Error Message : " << msg << "\n";
        std::cerr << "Error Location : line " << ln << " col " << col << "\n\n";

        std::cerr << line << std::endl;
        std::cerr << std::string(col - 1, ' ') << "^\n";
    };

    parser.log = on_error;

    Dlink::StatementPtr ast;

    if (parser.parse(input.c_str(), ast))
    {
        std::cout << "Parsing Succeeded\n\n";
        std::cout << "<Generated Parse Tree>\n";
        std::cout << ast->tree_gen(0) << "\n\n";
 
        std::cout << "<Generated LLVM IR>\n";
        ast->code_gen(); 
        Dlink::LLVM::module->dump();

        return 0;
    }
    else
    {
        return -1;
    }
}
