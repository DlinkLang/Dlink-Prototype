#include <iostream>

#include "peglib.hh"

int main(int argc, const char** argv)
{
	if (argc < 2)
	{
		std::cout << "usage: calc [formula]" << std::endl;
		return 1;
	}

	using NumType = int;
	auto eval = [](const peg::SemanticValues& v)
	{
		NumType result = v[0].get<NumType>();

		for (std::size_t i=1; i < v.size(); i += 2)
		{
			NumType num = v[i+1].get<NumType>();
			char op = v[i].get<char>();

			switch(op)
			{
				case '+':
					result += num;
					break;
				case '-':
					result -= num;
					break;
				case '*':
					result *= num;
					break;
				case '/':
					result /= num;
					break;
			}
		}

		return result;
	};

	peg::parser calculator(R"(
                EXPR      <- _ TERM (TERM_OP TERM)*
                TERM      <- FACTOR (FACTOR_OP FACTOR)*
                FACTOR    <- NUMBER / '(' _ EXPR ')' _
                TERM_OP   <- < [-+] > _
                FACTOR_OP <- < [/*] > _
                NUMBER    <- < [0-9]+ > _
                ~_        <- [ \t\r\n]*
    )");

	calculator["EXPR"]      = eval;
	calculator["TERM"]      = eval;
	calculator["TERM_OP"]   = [](const peg::SemanticValues& v) { return static_cast<char>(*v.c_str()); };
	calculator["FACTOR_OP"] = [](const peg::SemanticValues& v) { return static_cast<char>(*v.c_str()); };
	calculator["NUMBER"]    = [](const peg::SemanticValues& v) { return std::atoi(v.c_str()); };

	const char* expr = argv[1];
	NumType result;

	if (calculator.parse(expr, result))
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
