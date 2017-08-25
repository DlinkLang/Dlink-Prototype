#include <iostream>

#include "Lexer.hh"
#include "Parser.hh"
#include "CodeGen.hh"

int main(int argc, const char** argv)
{
	Dlink::LLVM::function_pm = std::make_unique<llvm::legacy::FunctionPassManager>(Dlink::LLVM::module.get());
	Dlink::LLVM::function_pm->doInitialization();

	Dlink::Lexer lexer;
	lexer.lex(R"(
	unsafe int main()
	{
		int i;
		int* ip = &i;
		int j = *ip;
		int* ip2 = &i;
	}
	)");

	std::cout << "Lexing Succeed\n";
	lexer.dump();
	std::cout << "\n";

	Dlink::TokenSeq token_seq = lexer.get_token_seq();

	Dlink::Parser parser(token_seq);

	Dlink::StatementPtr ast;
	if (parser.parse(ast))
	{
		for(auto warning : parser.get_warnings())
		{
			Dlink::Token warning_token = warning.message_token();
			std::cerr << "Warning at ";
			std::cerr << "Line " << warning_token.line;
			std::cerr << " Col " << warning_token.col;

			std::cerr << " " << warning.what() << '\n';
		}

		std::cout << "Parsing Succeed\n";
		std::string temp = ast->tree_gen(0);
		std::cout << ast->tree_gen(0) << "\n\n";

		try
		{
			ast->code_gen();

			for(auto warning : Dlink::CompileMessage::warnings.get_warnings())
			{
				Dlink::Token warning_token = warning.message_token();
				std::cerr << "Warning at ";
				std::cerr << "Line " << warning_token.line;
				std::cerr << " Col " << warning_token.col;

				std::cerr << " " << warning.what() << '\n';
			}

			std::cout << "Code generation Succeed\n";
			Dlink::LLVM::module->dump();
		}
		catch (const Dlink::Error& error)
		{
			std::cerr << "Code generation Failed\n";

			Dlink::Token error_token = error.message_token();
			std::cerr << "Error at ";
			std::cerr << "Line " << error_token.line;
			std::cerr << " Col " << error_token.col;
			
			std::cerr << " " << error.what() << '\n';
		}
	}
	else
	{
		std::cout << "Parsing Failed\n";

		for (auto error : parser.get_errors())
		{
			Dlink::Token error_token = error.message_token();
			std::cerr << "Error at ";
			std::cerr << "Line " << error_token.line;
			std::cerr << " Col " << error_token.col;

			std::cerr << " " << error.what() << '\n';
		}
	}

#ifdef _MSC_VER
	std::system("pause");
#endif
	return 0;
}
