#include <iostream>

#include "Lexer.hh"
#include "Parser.hh"
#include "CodeGen.hh"

int main(int argc, const char** argv)
{
	Dlink::Lexer lexer;
	lexer.lex(R"(
	unsigned i = 5;
	)");

	std::cout << "Lexing Succeed\n";
	lexer.dump();
	std::cout << "\n";

	Dlink::TokenSeq token_seq = lexer.get_token_seq();

	Dlink::Parser parser(token_seq);

	Dlink::StatementPtr ast;
	if (parser.parse(ast))
	{
		std::cout << "Parsing Succeed\n";
		std::cout << ast->tree_gen(0) << '\n';

		try
		{
			ast->code_gen();

			std::cout << "Code generation Succeed\n";
			Dlink::LLVM::module->dump();
		}
		catch (...)
		{
			std::cout << "Code generation Failed\n";
			return 0;
		}
	}
	else
	{
		std::cout << "Parsing Failed\n";

		for (auto error : parser.get_errors())
		{
			Dlink::Token error_token = error.error_token();
			std::cout << "Line " << error_token.line;
			std::cout << " Col " << error_token.col;

			std::cout << " " << error.what() << "\n";
		}
	}

#ifdef _MSC_VER
	std::system("pause");
#endif
	return 0;
}
