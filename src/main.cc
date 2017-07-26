#include <iostream>

#include "peglib.hh"

#include "Parser.hh"

int main(int argc, const char** argv)
{
	if (argc < 2)
	{
		std::cout << "usage: calc [formula]" << std::endl;
		return 1;
	}

	Dlink::Parser parser;

	const char* expr = argv[1];
	int result;

	if (parser.parse(expr, result))
	{
		std::cout << expr << " = " << result << std::endl;
		return 0;
	}
	else
	{
		std::cerr << "Syntax Error!\n";
		return -1;
	}
}
