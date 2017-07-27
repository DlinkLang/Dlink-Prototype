#include "Parser.hh"

namespace Dlink
{
	const constexpr char Parser::grammar[] = R"(
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
		auto BinaryOP_AST = [](const peg::SemanticValues& v) -> ExpressionPtr
		{	
			ExpressionPtr lhs = v[0].get<ExpressionPtr>();

			for (std::size_t i=1; i < v.size(); i += 2)
			{
				auto rhs = v[i+1].get<ExpressionPtr>();
				auto op = v[i].get<BinaryOperator>();

				lhs = std::make_shared<BinaryOP>(op, lhs, rhs);
			}
			
			return lhs;
		};
		
		load_grammar(grammar);
		
		auto parser = *this;

		parser["EXPR"]      = BinaryOP_AST;
		parser["TERM"]      = BinaryOP_AST;
		parser["TERM_OP"]   = [](const peg::SemanticValues& v) -> BinaryOperator
		{ 
			char op_ch = static_cast<char>(*v.c_str()); 
			
			if(op_ch == '-')
			{
				return BinaryOperator::Minus;
			}
			else if(op_ch == '+')
			{
				return BinaryOperator::Plus;
			}

			return BinaryOperator::None;
		};

		parser["FACTOR_OP"] = [](const peg::SemanticValues& v) -> BinaryOperator
		{ 
			char op_ch = static_cast<char>(*v.c_str()); 
			
			if(op_ch == '/')
			{
				return BinaryOperator::Divide;
			}
			else if(op_ch == '*')
			{
				return BinaryOperator::Multiply;
			}

			return BinaryOperator::None;
		};

		parser["NUMBER"]    = [](const peg::SemanticValues& v) -> ExpressionPtr
		{ 
			int32_t data = std::atoi(v.c_str()); 
			ExpressionPtr node = std::make_shared<Integer32>(data);
			return node;
		};
	}
}
