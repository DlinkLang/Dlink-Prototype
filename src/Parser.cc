#include "Parser.hh"

namespace Dlink
{
	/**
	 * @brief 새 Parser 인스턴스를 만듭니다.
	 * @param input 렉서를 통해 만들어진 토큰 목록입니다.
	 */
	Parser::Parser(const TokenSeq& input)
		: input_(input), token_iter_(input_.cbegin())
	{}

	/**
	 * @brief 토큰 목록을 이용해 파싱한 후 추상 구문 트리를 만듭니다.
	 * @details 생성자를 통해 입력받은 토큰 목록을 사용합니다.
	 * @param output 만들어진 추상 구문 트리를 저장할 변수입니다.
	 * @return 파싱에 성공하면 true, 실패하면 false를 반환합니다.
	 * @see Dlink::Parser::Parser(const TokenSeq&)
	 */
	bool Parser::parse(StatementPtr& output)
	{
		return block(output);
	}
	/**
	 * @brief 파싱을 하는 도중 발생한 에러 집합을 가져옵니다.
	 * @details 이 함수는 예외를 발생시키지 않습니다.
	 * @return 에러 집합을 반환합니다.
	 */
	const std::vector<Error>& Parser::get_errors() const noexcept
	{
		return errors_.get_errors();
	}

	Token Parser::current_token() const
	{
		return *token_iter_;
	}
	Token Parser::previous_token() const
	{
		return *(token_iter_ - 1);
	}
	Token Parser::next_token() const
	{
		return *(token_iter_ + 1);
	}
	bool Parser::accept(TokenType token_type)
	{
		if ((*token_iter_).type == token_type)
		{
			token_iter_++;
			return true;
		}

		return false;
	}
}

namespace Dlink
{
	bool Parser::block(StatementPtr& out)
	{
		std::vector<StatementPtr> statements;
		StatementPtr statement;

		while (/*scope*/var_decl(statement))
		{
			statements.push_back(statement);
			statement = nullptr;
		}

		out = std::make_shared<Block>(statements);
		return true;
	}

	bool Parser::scope(StatementPtr& out)
	{
		if (accept(TokenType::lbrace))
		{
			std::vector<StatementPtr> statements;
			StatementPtr statement;
			while (var_decl(statement))
			{
				statements.push_back(statement);
				statement = nullptr;
			}

			out = std::make_shared<Scope>(statements, nullptr/* TODO: it's temp */);
		}
		else
		{
			StatementPtr statement;

			if (var_decl(statement))
			{
				out = statement;
				return true;
			}

			return false;
		}

		return true;
	}

	bool Parser::var_decl(StatementPtr& out)
	{
		TypePtr type_expr;

		if (type(type_expr))
		{
			if (accept(TokenType::identifier))
			{
				std::string name = previous_token().data;

				if (accept(TokenType::assign))
				{
					ExpressionPtr expression;

					if (expr(expression))
					{
						out = std::make_shared<VariableDeclaration>(type_expr, name, expression);
						return true;
					}
					else
					{
						errors_.add_error(Error(current_token(), "Expected expression, but got \"" + current_token().data + "\""));
						return false;
					}
				}
				else if (accept(TokenType::semicolon))
				{
					out = std::make_shared<VariableDeclaration>(type_expr, name);
					return true;
				}
			}

			errors_.add_error(Error(current_token(), "Expected identifier, but got \"" + current_token().data + "\""));
			return false;
		}
		else
		{
			StatementPtr statement;

			if (expr_stmt(statement))
			{
				out = statement;
				return true;
			}

			return false;
		}
	}
	
	bool Parser::expr_stmt(StatementPtr& out)
	{
		ExpressionPtr expression;

		if(!expr(expression))
		{
			errors_.add_error(Error(current_token(), "Expected identifier, but got \"" + current_token().data + "\""));
			return false;
		}

		if(accept(TokenType::semicolon))
		{
			out = std::make_shared<ExpressionStatement>(expression);
			return true;
		}
		else
		{
			errors_.add_error(Error(current_token(), "Expected ';', but got \"" + current_token().data + "\""));
			return false;
		}
	}
}

namespace Dlink
{
	bool Parser::expr(ExpressionPtr& out)
	{
		return assign(out);
	}

	bool Parser::assign(ExpressionPtr& out)
	{
		ExpressionPtr lhs;

		if (!addsub(lhs))
		{
			return false;
		}

		TokenType op;
		
		std::vector<ExpressionPtr> operands;
		operands.push_back(lhs);

		while (accept(TokenType::assign))
		{
			op = previous_token().type;

			ExpressionPtr rhs;
			if (!addsub(rhs))
			{
				errors_.add_error(Error(current_token(), "Expected expression, but got \"" + current_token().data + "\""));
				return false;
			}
			
			operands.push_back(rhs);
		}
		
		ExpressionPtr result;

		result = operands.back();
		operands.pop_back();
		std::reverse(operands.begin(), operands.end());

		for(ExpressionPtr operand : operands)
		{
			result = std::make_shared<BinaryOperation>(TokenType::assign, result, operand);
		}

		out = result;
		return true;
	}

	bool Parser::addsub(ExpressionPtr& out)
	{
		ExpressionPtr lhs;

		if (!muldiv(lhs))
		{
			return false;
		}

		TokenType op;

		while (accept(TokenType::plus) || accept(TokenType::minus))
		{
			op = previous_token().type;

			ExpressionPtr rhs;
			if (!muldiv(rhs))
			{
				errors_.add_error(Error(current_token(), "Expected expression, but got \"" + current_token().data + "\""));
				return false;
			}

			lhs = std::make_shared<BinaryOperation>(op, lhs, rhs);
		}

		out = lhs;
		return true;
	}

	bool Parser::muldiv(ExpressionPtr& out)
	{
		ExpressionPtr lhs;

		if (!atom(lhs))
		{
			return false;
		}

		TokenType op;

		while (accept(TokenType::multiply) || accept(TokenType::divide))
		{
			op = previous_token().type;

			ExpressionPtr rhs;
			if (!number(rhs))
			{
				errors_.add_error(Error(current_token(), "Expected expression, but got \"" + current_token().data + "\""));
				return false;
			}

			lhs = std::make_shared<BinaryOperation>(op, lhs, rhs);
		}

		out = lhs;
		return true;
	}

	bool Parser::atom(ExpressionPtr& out)
	{
		return number(out) || identifier(out);
	}
}

namespace Dlink
{
	bool Parser::number(ExpressionPtr& out)
	{
		if (accept(TokenType::dec_integer))
		{
			out = std::make_shared<Integer32>(std::stoi(previous_token().data));
			return true;
		}

		return false;
	}

	bool Parser::identifier(ExpressionPtr& out)
	{
		if (accept(TokenType::identifier))
		{
			out = std::make_shared<Identifer>(previous_token().data);
			return true;
		}

		return false;
	}
}

namespace Dlink
{
	bool Parser::type(TypePtr& out)
	{
		return simple_type(out);
	}

	bool Parser::simple_type(TypePtr& out)
	{
		if (accept(TokenType::_unsigned))
		{
			if (accept(TokenType::_char))
			{
				// unsigned char
				return false; // TODO: 아직 구현되지 않음
			}
			else if (accept(TokenType::_short))
			{
				// unsigned short
				return false; // TODO: 아직 구현되지 않음
			}
			else if (accept(TokenType::_int))
			{
				// unsigned int
				out = std::make_shared<SimpleType>("int", true);
				return true;
			}
			else if (accept(TokenType::_long))
			{
				// unsigned long
				return false; // TODO: 아직 구현되지 않음
			}
			else
			{
				// unsigned int
				out = std::make_shared<SimpleType>("int", true);
				return true;
			}
		}
		else if (accept(TokenType::_signed))
		{
			if (accept(TokenType::_char))
			{
				// signed char
				return false; // TODO: 아직 구현되지 않음
			}
			else if (accept(TokenType::_short))
			{
				// signed short
				return false; // TODO: 아직 구현되지 않음
			}
			else if (accept(TokenType::_int))
			{
				// signed int
				out = std::make_shared<SimpleType>("int");
				return true;
			}
			else if (accept(TokenType::_long))
			{
				// signed long
				return false; // TODO: 아직 구현되지 않음
			}
			else
			{
				// signed int
				out = std::make_shared<SimpleType>("int");
				return true;
			}
		}
		
		else if (accept(TokenType::_char))
		{
			// char
			return false; // TODO: 아직 구현되지 않음
		}
		else if (accept(TokenType::_short))
		{
			// short
			return false; // TODO: 아직 구현되지 않음
		}
		else if (accept(TokenType::_int))
		{
			// int
			out = std::make_shared<SimpleType>("int");
			return true;
		}
		else if (accept(TokenType::_long))
		{
			// long
			return false; // TODO: 아직 구현되지 않음
		}

		return false;
	}
}
