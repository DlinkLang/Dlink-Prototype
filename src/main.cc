#include <iostream>

#include "Init.hh"
#include "CommandLine.hh"
#include "Lexer.hh"
#include "Parser.hh"
#include "CodeGen.hh"

int main(int argc, char** argv)
{
	std::string code;

	try
	{
		code = Dlink::ProcessCommandLine(argc, argv);
	} 
	catch(Dlink::ParsedCommandLine::Error e) 
	{
		switch (e)
		{
			case Dlink::ParsedCommandLine::Error::Invalid_Value:
				std::cerr << "fatal: invalid value\n";
				break;
			case Dlink::ParsedCommandLine::Error::CouldntFind_Input:
				std::cerr << "fatal: couldn't find input file\n";
				break;
			case Dlink::ParsedCommandLine::Error::Multi_Optimize:
				std::cerr << "fatal: unexpected multiple optimization options\n";
				break;
			case Dlink::ParsedCommandLine::Error::Multi_IR:
				std::cerr << "fatal: unexpected multiple ir output options\n";
				break;
			case Dlink::ParsedCommandLine::Error::No_Input:
				std::cerr << "fatal: no input\n";
				break;
			case Dlink::ParsedCommandLine::Error::Unknown:
				std::cerr << "fatal: unknown option\n";
				break;
			default:
				std::cerr << "fatal: unhandled error caught\n";
				break;
		}

		return -1;
	}

	Dlink::Lexer lexer;
	lexer.lex(code);

	std::cout << "Lexing Succeed\n";
	lexer.dump();
	std::cout << "\n";

	Dlink::TokenSeq token_seq = lexer.get_token_seq();

	Dlink::Parser parser(token_seq);

	Dlink::StatementPtr ast;
	if (parser.parse(ast))
	{
		for (auto warning : parser.get_warnings())
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

			for (auto warning : Dlink::CompileMessage::warnings.get_warnings())
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
