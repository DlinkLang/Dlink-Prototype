#include "Parser.hh"

namespace Dlink
{
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
	Parser::Parser(const TokenSeq& input)
		: input_(input), token_iter_(input_.cbegin())
	{}

	bool Parser::parse(ExpressionPtr& output)
	{
		return expr(output);
	}

	std::vector<Error> Parser::get_errors() const noexcept
	{
		return errors_.get_errors();
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

		while (accept(TokenType::assign))
		{
			op = previous_token().type;

			ExpressionPtr rhs;
			if (!addsub(rhs))
			{
				// TODO: 에러 메세지 넣어주세요
				errors_.add_error(Error(current_token(), "TODO"));
				return false;
			}

			lhs = std::make_shared<BinaryOperation>(op, lhs, rhs);
		}

		out = lhs;
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