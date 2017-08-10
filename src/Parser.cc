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
	 * @param output 만들어진 추상 구문 트리를 저장할	 변수입니다.
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

			out = std::make_shared<Scope>(statements);
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
	}

	bool Parser::var_decl(StatementPtr& out)
	{
		if (accept(TokenType::identifier))
		{
			TypePtr type_expr;

			--token_iter_;
			if (!type(type_expr))
			{
				// TODO: 에러 메세지 넣어주세요
				errors_.add_error(Error(current_token(), "TODO"));
				return false;
			}

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
						// TODO: 에러 메세지 넣어주세요
						errors_.add_error(Error(current_token(), "TODO"));
						return false;
					}
				}
				else if (accept(TokenType::semicolon))
				{
					out = std::make_shared<VariableDeclaration>(type_expr, name);
					return true;
				}
			}
		}

		// TODO: 에러 메세지 넣어주세요
		errors_.add_error(Error(current_token(), "TODO"));
		return false;
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

namespace Dlink
{
	bool Parser::type(TypePtr& out)
	{
		return simple_type(out);
	}

	bool Parser::simple_type(TypePtr& out)
	{
		if (accept(TokenType::identifier))
		{
			out = std::make_shared<SimpleType>(previous_token().data);
			return true;
		}

		return false;
	}
}