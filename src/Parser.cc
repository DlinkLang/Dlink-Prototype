#include "Parser.hh"

namespace Dlink
{
	const char Parser::grammar[] = R"(
                EXPR      <- _ TERM (TERM_OP TERM)*
                TERM      <- FACTOR (FACTOR_OP FACTOR)*
                FACTOR    <- NUMBER / '(' _ EXPR ')' _
                TERM_OP   <- < [-+] > _
                FACTOR_OP <- < [/*] > _
                NUMBER    <- < [0-9]+ > _
                ~_        <- [ \t\r\n]*
    )";

	Parser::Parser()
	{
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
		
		load_grammar(grammar);
		
		auto parser = *this;

		parser["EXPR"]      = eval;
		parser["TERM"]      = eval;
		parser["TERM_OP"]   = [](const peg::SemanticValues& v) { return static_cast<char>(*v.c_str()); };
		parser["FACTOR_OP"] = [](const peg::SemanticValues& v) { return static_cast<char>(*v.c_str()); };
		parser["NUMBER"]    = [](const peg::SemanticValues& v) { return std::atoi(v.c_str()); };
	}
}
