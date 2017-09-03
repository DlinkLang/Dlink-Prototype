#include "Parser.hh"
#include "CodeGen.hh"

namespace Dlink
{
	/**
	 * @brief 새 Parser 인스턴스를 만듭니다.
	 * @param input 렉서를 통해 만들어진 토큰 목록입니다.
	 */
	Parser::Parser(const TokenSeq& input)
		: token_seq_(input), token_iter_(token_seq_.cbegin())
	{}

	/**
	 * @brief 토큰 목록을 이용해 파싱한 후 추상 구문 트리를 만듭니다.
	 * @details 생성자를 통해 입력받은 토큰 목록을 사용합니다.
	 * @return 파싱에 성공하면 true, 실패하면 false를 반환합니다.
	 * @see Dlink::Parser::Parser(const TokenSeq&)
	 */
	bool Parser::parse()
	{
		StatementPtr statement;
		bool ret = block(statement);

		ast_.node_ = statement;
		return ret;
	}
	/**
	 * @brief 파싱 작업을 통해 만들어진 추상 구문 트리를 가져옵니다.
	 * @details 이 함수는 예외를 발생시키지 않습니다.
	 * @return 추상 구문 트리를 반환합니다.
	 */
	AST& Parser::get_ast() noexcept
	{
		return ast_;
	}
	/**
	* @brief 파싱 작업을 통해 만들어진 추상 구문 트리를 가져옵니다.
	* @details 이 함수는 예외를 발생시키지 않습니다.
	* @return 추상 구문 트리를 반환합니다.
	*/
	const AST& Parser::get_ast() const noexcept
	{
		return ast_;
	}
	/**
	 * @brief 파싱을 하는 도중 발생한 에러 집합을 가져옵니다.
	 * @details 이 함수는 예외를 발생시키지 않습니다.
	 * @return 에러 집합을 반환합니다.
	 */
	const Errors& Parser::get_errors() const noexcept
	{
		return errors_;
	}
	/**
	 * @brief 파싱을 하는 도중 발생한 경고 집합을 가져옵니다.
	 * @details 이 함수는 예외를 발생시키지 않습니다.
	 * @return 경고 집합을 반환합니다.
	 */
	const Warnings& Parser::get_warnings() const noexcept
	{
		return warnings_;
	}

	void Parser::assign_token(Token* dest, Token source)
	{
		if (dest)
		{
			*dest = source;
		}
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
	bool Parser::accept(TokenType token_type, Token* start_token)
	{
		if ((*token_iter_).type == token_type)
		{
			token_iter_++;

			assign_token(start_token, previous_token());
			return true;
		}

		return false;
	}
}

namespace Dlink
{
	bool Parser::block(StatementPtr& out, Token* start_token)
	{
		std::vector<StatementPtr> statements;
		StatementPtr statement;

		Token block_start;
		while (scope(statement, &block_start))
		{
			statements.push_back(statement);
			statement = nullptr;
		}

		if (errors_.get_errors().empty())
		{
			out = std::make_shared<Block>(block_start, statements);

			assign_token(start_token, block_start);
			return true;
		}
		else
		{
			return false;
		}
	}

	bool Parser::scope(StatementPtr& out, Token* start_token)
	{
		Token scope_start;
		if (accept(TokenType::lbrace, &scope_start))
		{
			std::vector<StatementPtr> statements;
			StatementPtr statement;
			while (scope(statement))
			{
				statements.push_back(statement);
				statement = nullptr;
			}

			if (accept(TokenType::rbrace))
			{
				out = std::make_shared<Scope>(scope_start, statements, nullptr/* TODO: it's temp */);

				assign_token(start_token, scope_start);
				return true;
			}
			else
			{
				errors_.add_error(Error(current_token(), "Expected '}', but got \"" + current_token().data + "\""));
				return false;
			}
		}
		else
		{
			StatementPtr statement;

			Token var_decl_start;
			if (var_decl(statement, &var_decl_start))
			{
				out = statement;

				assign_token(start_token, var_decl_start);
				return true;
			}

			return false;
		}

		return true;
	}

	bool Parser::var_decl(StatementPtr& out, Token* start_token)
	{
		TypePtr type_expr;

		Token unsafe_start;
		bool is_unsafe = false;
		if (accept(TokenType::unsafe, &unsafe_start))
		{
			is_unsafe = true;
		}

		Token var_decl_start;
		if (type(type_expr, &var_decl_start))
		{
			if (accept(TokenType::identifier))
			{
				std::string name = previous_token().data;

				if (accept(TokenType::assign))
				{
					ExpressionPtr expression;

					if (expr(expression))
					{
						if (accept(TokenType::semicolon))
						{
							StatementPtr var = std::make_shared<VariableDeclaration>(var_decl_start, type_expr, name, expression);

							if (is_unsafe)
							{
								out = std::make_shared<UnsafeStatement>(unsafe_start, var);
								assign_token(start_token, unsafe_start);
							}
							else
							{
								out = var;
								assign_token(start_token, var_decl_start);
							}

							return true;
						}
						else
						{
							errors_.add_error(Error(current_token(), "Expected ';', but got \"" + current_token().data + "\""));
							return false;
						}
					}
					else
					{
						errors_.add_error(Error(current_token(), "Expected expression, but got \"" + current_token().data + "\""));
						return false;
					}
				}
				else if (accept(TokenType::semicolon))
				{
					StatementPtr var = std::make_shared<VariableDeclaration>(var_decl_start, type_expr, name);

					if (is_unsafe)
					{
						out = std::make_shared<UnsafeStatement>(unsafe_start, var);
						assign_token(start_token, unsafe_start);
					}
					else
					{
						out = var;
						assign_token(start_token, var_decl_start);
					}

					return true;
				}
				else if (accept(TokenType::lparen))
				{
					return func_decl(out, var_decl_start, type_expr, name, unsafe_start, is_unsafe);
				}
			}

			errors_.add_error(Error(current_token(), "Expected identifier, but got \"" + current_token().data + "\""));
			return false;
		}
		else
		{
			if (is_unsafe)
			{
				errors_.add_error(Error(current_token(), "Unexpected \"unsafe\""));
				return false;
			}

			StatementPtr statement;

			Token return_start;
			if (return_stmt(statement, &return_start))
			{
				out = statement;

				assign_token(start_token, return_start);
				return true;
			}

			return false;
		}
	}

	bool Parser::func_decl(StatementPtr& out, Token var_decl_start_token, TypePtr return_type, const std::string& identifier, Token unsafe_start, bool is_unsafe, Token* start_token)
	{
		std::vector<VariableDeclaration> param_list;

		while (true)
		{
			TypePtr param_type;
			if (type(param_type))
			{
				if (param_type->token.type == TokenType::_void)
				{
					if (accept(TokenType::rparen))
						break;
					else
					{
						errors_.add_error(Error(current_token(), "Unexpected additional parameter in void paramter function"));
						return false;
					}
				}
				else
				{
					if (accept(TokenType::identifier))
					{
						VariableDeclaration param(var_decl_start_token, param_type, previous_token().data);
						param_list.push_back(param);

						if (accept(TokenType::comma))
							continue;
					}
					else if (accept(TokenType::comma))
					{
						VariableDeclaration param(var_decl_start_token, param_type, "");
						param_list.push_back(param);
						continue;
					}
				}
			}
			else if (accept(TokenType::rparen))
				break;
			else
			{
				errors_.add_error(Error(current_token(), "Unexpected \"" + current_token().data + "\""));
				return false;
			}
		}

		StatementPtr body;

		if (!scope(body))
		{
			errors_.add_error(Error(current_token(), "Unexpected \"" + current_token().data + "\""));
			return false;
		}

		StatementPtr func = std::make_shared<FunctionDeclaration>(var_decl_start_token, return_type, identifier, param_list, body);

		if (is_unsafe)
		{
			out = std::make_shared<UnsafeStatement>(unsafe_start, func);
			assign_token(start_token, unsafe_start);
		}
		else
		{
			out = func;
			assign_token(start_token, var_decl_start_token);
		}

		return true;
	}

	bool Parser::return_stmt(StatementPtr& out, Token* start_token)
	{
		Token return_start;
		if (accept(TokenType::_return, &return_start))
		{
			ExpressionPtr return_expr;

			expr(return_expr);

			if (accept(TokenType::semicolon))
			{
				out = std::make_shared<ReturnStatement>(return_start, return_expr);

				assign_token(start_token, return_start);
				return true;
			}
			else
			{
				errors_.add_error(Error(current_token(), "Expected ';', but got \"" + current_token().data + "\""));
				return false;
			}
		}
		else
		{
			StatementPtr statement;

			Token expr_stmt_start;
			if (expr_stmt(statement, &expr_stmt_start))
			{
				out = statement;

				assign_token(start_token, expr_stmt_start);
				return true;
			}

			return false;
		}
	}

	bool Parser::unsafe_stmt(StatementPtr& out, Token* start_token)
	{
		// TODO

		return false;
	}

	bool Parser::expr_stmt(StatementPtr& out, Token* start_token)
	{
		ExpressionPtr expression;

		Token expr_stmt_start;
		if (!expr(expression, &expr_stmt_start))
		{
			return false;
		}

		if (accept(TokenType::semicolon))
		{
			out = std::make_shared<ExpressionStatement>(expr_stmt_start, expression);

			assign_token(start_token, expr_stmt_start);
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
	bool Parser::expr(ExpressionPtr& out, Token* start_token)
	{
		Token unsafe_start;
		if (accept(TokenType::unsafe, &unsafe_start))
		{
			ExpressionPtr expr;
			if (!assign(expr))
			{
				errors_.add_error(Error(current_token(), "Expected expression, but got \"" + current_token().data + "\""));
				return false;
			}

			out = std::make_shared<UnsafeExpression>(unsafe_start, expr);
			assign_token(start_token, unsafe_start);
			return true;
		}
		else
		{
			return assign(out, start_token);
		}
	}

	bool Parser::assign(ExpressionPtr& out, Token* start_token)
	{
		ExpressionPtr lhs;

		Token assign_start;
		if (!addsub(lhs, &assign_start))
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

		for (ExpressionPtr operand : operands)
		{
			result = std::make_shared<BinaryOperation>(assign_start, TokenType::assign, operand, result);
		}

		out = result;

		assign_token(start_token, assign_start);
		return true;
	}

	bool Parser::addsub(ExpressionPtr& out, Token* start_token)
	{
		ExpressionPtr lhs;

		Token addsub_start;
		if (!muldiv(lhs, &addsub_start))
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

			lhs = std::make_shared<BinaryOperation>(addsub_start, op, lhs, rhs);
		}

		out = lhs;

		assign_token(start_token, addsub_start);
		return true;
	}

	bool Parser::muldiv(ExpressionPtr& out, Token* start_token)
	{
		ExpressionPtr lhs;

		Token muldiv_start;
		if (!unary(lhs, &muldiv_start))
		{
			return false;
		}

		TokenType op;

		while (accept(TokenType::multiply) || accept(TokenType::divide))
		{
			op = previous_token().type;

			ExpressionPtr rhs;
			if (!unary(rhs))
			{
				errors_.add_error(Error(current_token(), "Expected expression, but got \"" + current_token().data + "\""));
				return false;
			}

			lhs = std::make_shared<BinaryOperation>(muldiv_start, op, lhs, rhs);
		}

		out = lhs;

		assign_token(start_token, muldiv_start);
		return true;
	}

	bool Parser::unary(ExpressionPtr& out, Token* start_token)
	{
		return unary_plusminus(out, start_token) || unary_address(out, start_token);
	}

	bool Parser::func_call(ExpressionPtr& out, Token* start_token)
	{
		ExpressionPtr func_expr;

		Token func_call_start;
		if (!paren(func_expr, &func_call_start))
		{
			return false;
		}

		while (accept(TokenType::lparen))
		{
			std::vector<ExpressionPtr> arg;

			while (true)
			{
				ExpressionPtr a_arg;
				if (expr(a_arg))
				{
					arg.push_back(a_arg);

					if (accept(TokenType::rparen))
					{
						func_expr = std::make_shared<FunctionCallOperation>(func_call_start, func_expr, arg);

						break;
					}
					else if (accept(TokenType::comma))
					{
						continue;
					}
					else
					{
						errors_.add_error(Error(current_token(), "Expected ',' or ';', but got \"" + current_token().data + "\""));
						return false;
					}
				}
				else if (accept(TokenType::rparen))
				{
					func_expr = std::make_shared<FunctionCallOperation>(func_call_start, func_expr, arg);

					break;
				}
			}
		}

		out = func_expr;
		assign_token(start_token, func_call_start);

		return true;
	}

	bool Parser::paren(ExpressionPtr& out, Token* start_token)
	{
		Token paren_start;
		if (accept(TokenType::lparen, &paren_start))
		{
			ExpressionPtr expression;
			expr(expression);

			if (accept(TokenType::rparen))
			{
				out = expression;

				assign_token(start_token, paren_start);
				return true;
			}
			else
			{
				errors_.add_error(Error(current_token(), "Expected ')', but got \"" + current_token().data + "\""));

				return false;
			}
		}
		else
		{
			ExpressionPtr array_list_expr;

			Token list_start;
			if (array_init_list(array_list_expr, &list_start))
			{
				out = array_list_expr;

				assign_token(start_token, list_start);
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	bool Parser::array_init_list(ExpressionPtr& out, Token* start_token)
	{
		Token list_start;
		if (accept(TokenType::lbrace, &list_start))
		{
			if (accept(TokenType::rbrace))
			{
				out = std::make_shared<ArrayInitList>(list_start, std::vector<ExpressionPtr>{});

				assign_token(start_token, list_start);
				return true;
			}

			std::vector<ExpressionPtr> elements;

			while (true)
			{
				ExpressionPtr expression;
				expr(expression);
				elements.push_back(expression);

				if (accept(TokenType::comma))
				{
					continue;
				}
				else if (accept(TokenType::rbrace))
				{
					break;
				}
				else
				{
					errors_.add_error(Error(current_token(), "Expected '}' or ',', but got \"" + current_token().data + "\""));

					return false;
				}
			}

			out = std::make_shared<ArrayInitList>(list_start, elements);

			assign_token(start_token, list_start);
			return true;
		}
		else
		{
			ExpressionPtr atom_expr;

			Token atom_start;
			if (atom(atom_expr, &atom_start))
			{
				out = atom_expr;

				assign_token(start_token, atom_start);
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	bool Parser::atom(ExpressionPtr& out, Token* start_token)
	{
		return number(out, start_token) 
			|| identifier(out, start_token)
			|| string(out, start_token)
			|| character(out, start_token);
	}
}

namespace Dlink
{
	bool Parser::unary_plusminus(ExpressionPtr& out, Token* start_token)
	{
		Token plusminus_start;
		if (accept(TokenType::plus, &plusminus_start) || accept(TokenType::minus, &plusminus_start))
		{
			TokenType op = previous_token().type;

			ExpressionPtr rhs;
			if (!func_call(rhs))
			{
				errors_.add_error(Error(current_token(), "Expected expression, but got \"" + current_token().data + "\""));
				return false;
			}

			out = std::make_shared<UnaryOperation>(plusminus_start, op, rhs);
			assign_token(start_token, plusminus_start);

			return true;
		}
		else
		{
			Token func_call_start;

			ExpressionPtr func_call_expr;
			if (func_call(func_call_expr, &func_call_start))
			{
				out = func_call_expr;

				assign_token(start_token, func_call_start);
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	bool Parser::unary_address(ExpressionPtr& out, Token* start_token)
	{
		Token address_start;
		if (accept(TokenType::multiply, &address_start) || accept(TokenType::bit_and, &address_start))
		{
			TokenType op = previous_token().type;

			ExpressionPtr rhs;
			if (!func_call(rhs))
			{
				errors_.add_error(Error(current_token(), "Expected expression, but got \"" + current_token().data + "\""));
				return false;
			}

			out = std::make_shared<UnaryOperation>(address_start, op, rhs);
			assign_token(start_token, address_start);

			return true;
		}

		return false;
	}

	bool Parser::number(ExpressionPtr& out, Token* start_token)
	{
		Token number_start;
		if (accept(TokenType::dec_integer, &number_start))
		{
			out = std::make_shared<Integer32>(number_start, std::stoi(previous_token().data));

			assign_token(start_token, number_start);
			return true;
		}

		return false;
	}

	bool Parser::identifier(ExpressionPtr& out, Token* start_token)
	{
		Token identifier_start;
		if (accept(TokenType::identifier, &identifier_start))
		{
			out = std::make_shared<Identifier>(identifier_start, previous_token().data);

			assign_token(start_token, identifier_start);
			return true;
		}

		return false;
	}

	std::string unescape(const std::string& s)
	{
		std::string res;
		std::string::const_iterator it = s.cbegin();

		while (it != s.cend())
		{
			char c = *it;
			it++;

			if (c == '\\' && it != s.end())
			{
				switch (*it) 
				{
					case '\\': 
						c = '\\'; 
						break;
					case 'n': 
						c = '\n'; 
						break;
					case 't': 
						c = '\t'; 
						break;
					default: 
						continue;
				}

				it++;

			}
			res += c;
		}

		return res;
	}
	bool Parser::string(ExpressionPtr& out, Token* start_token)
	{
		Token string_start;
		if (accept(TokenType::string, &string_start))
		{
			out = std::make_shared<String>(string_start, unescape(previous_token().data));

			assign_token(start_token, string_start);
			return true;
		}

		return false;
	}

	bool Parser::character(ExpressionPtr& out, Token* start_token)
	{
		Token char_start;
		if (accept(TokenType::character, &char_start))
		{
			out = std::make_shared<Character>(char_start, unescape(previous_token().data)[0]);

			assign_token(start_token, char_start);
			return true;
		}

		return false;
	}
}

namespace Dlink
{
	bool Parser::type(TypePtr& out, Token* start_token)
	{
		return array_type(out, start_token);
	}

	bool Parser::array_type(TypePtr& out, Token* start_token)
	{
		Token array_start_token;
		TypePtr type;

		if (reference_type(type, &array_start_token))
		{
			TypePtr lhs_array_type = type;

			if (accept(TokenType::lbparen))
			{
			loop:
				ExpressionPtr length;
				if (expr(length))
				{
					if (accept(TokenType::rbparen))
					{
						lhs_array_type = std::make_shared<StaticArray>(array_start_token, lhs_array_type, length);

						if (accept(TokenType::lbparen))
							goto loop;

						out = lhs_array_type;
						assign_token(start_token, array_start_token);
						return true;
					}
				}
				else
				{
					errors_.add_error(Error(current_token(), "Expected expression, but got \"" + current_token().data + "\""));
					return false;
				}
			}
			else
			{
				out = type;

				assign_token(start_token, array_start_token);
				return true;
			}
		}

		return false;
	}

	bool Parser::reference_type(TypePtr& out, Token* start_token)
	{
		Token pointer_start_token;
		TypePtr type;

		if (pointer_type(type, &pointer_start_token))
		{
			if (accept(TokenType::bit_and))
			{
				out = std::make_shared<LValueReference>(pointer_start_token, type);

				assign_token(start_token, pointer_start_token);
				return true;
			}
			else
			{
				out = type;

				assign_token(start_token, pointer_start_token);
				return true;
			}
		}

		return false;
	}

	bool Parser::pointer_type(TypePtr& out, Token* start_token)
	{
		Token pointer_start;
		TypePtr pointer;

		if (!simple_type(pointer, &pointer_start))
		{
			return false;
		}

		while (accept(TokenType::multiply))
		{
			pointer = std::make_shared<Pointer>(pointer_start, pointer);
		}

		out = pointer;
		assign_token(start_token, pointer_start);

		return true;
	}

	bool Parser::simple_type(TypePtr& out, Token* start_token)
	{
		Token simple_type_start;
		if (accept(TokenType::_unsigned, &simple_type_start))
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
				out = std::make_shared<SimpleType>(simple_type_start, "int", true);

				assign_token(start_token, simple_type_start);
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
				out = std::make_shared<SimpleType>(simple_type_start, "int", true);

				assign_token(start_token, simple_type_start);
				return true;
			}
		}
		else if (accept(TokenType::_signed, &simple_type_start))
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
				out = std::make_shared<SimpleType>(simple_type_start, "int");

				assign_token(start_token, simple_type_start);
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
				out = std::make_shared<SimpleType>(simple_type_start, "int");

				assign_token(start_token, simple_type_start);
				return true;
			}
		}

		else if (accept(TokenType::_char, &simple_type_start))
		{
			// char
			return false; // TODO: 아직 구현되지 않음
		}
		else if (accept(TokenType::_short, &simple_type_start))
		{
			// short
			return false; // TODO: 아직 구현되지 않음
		}
		else if (accept(TokenType::_int, &simple_type_start))
		{
			// int
			out = std::make_shared<SimpleType>(simple_type_start, "int");

			assign_token(start_token, simple_type_start);
			return true;
		}
		else if (accept(TokenType::_long, &simple_type_start))
		{
			// long
			return false; // TODO: 아직 구현되지 않음
		}
		else if (accept(TokenType::_bool, &simple_type_start))
		{
			// bool
			out = std::make_shared<SimpleType>(simple_type_start, "bool");

			assign_token(start_token, simple_type_start);
			return true;
		}
		else if (accept(TokenType::_void, &simple_type_start))
		{
			// void
			out = std::make_shared<SimpleType>(simple_type_start, "void");

			assign_token(start_token, simple_type_start);
			return true;
		}

		return false;
	}
}
