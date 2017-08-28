#pragma once

/**
 * @file Parser.hh
 * @author dev_kr, kmc7468
 * @brief Parser 클래스를 정의합니다.
 */

#include <iostream>
#include <set>
#include <vector>

#include "Message/Error.hh"
#include "Message/Warning.hh"
#include "ParseStruct.hh"
#include "Token.hh"

namespace Dlink
{
	/**
	 * @brief 렉서를 통해 만들어진 토큰으로 추상 구문 트리를 만드는 파서입니다.
	 * @details 이 클래스는 다른 곳에서 상속받을 수 없습니다.
	 * @see Dlink::Lexer
	 */
	class Parser final
	{
	public:
		Parser(const TokenSeq& input);
		Parser(const Parser& parser) = delete;
		Parser(Parser&& parser) noexcept = delete;
		~Parser() = default;

	public:
		Parser& operator=(const Parser& parser) = delete;
		Parser& operator=(Parser&& parser) noexcept = delete;
		bool operator==(const Parser& parser) const noexcept = delete;
		bool operator!=(const Parser& parser) const noexcept = delete;

	public:
		bool parse();
		AST& get_ast() noexcept;
		const AST& get_ast() const noexcept;
		const Errors& get_errors() const noexcept;
		const Warnings& get_warnings() const noexcept;

	private:
		void assign_token(Token* dest, Token source);

		Token current_token() const;
		Token previous_token() const;
		Token next_token() const;
		bool accept(TokenType token_type, Token* start_token = nullptr);

		bool block(StatementPtr& out, Token* start_token = nullptr);
		bool scope(StatementPtr& out, Token* start_token = nullptr);
		bool var_decl(StatementPtr& out, Token* start_token = nullptr);
		bool func_decl(StatementPtr& out, Token var_decl_start_token, TypePtr return_type, const std::string& identifier,
					   Token unsafe_start, bool is_unsafe, Token* start_token = nullptr);
		bool return_stmt(StatementPtr& out, Token* start_token = nullptr);
		bool unsafe_stmt(StatementPtr& out, Token* start_token = nullptr);
		bool expr_stmt(StatementPtr& out, Token* start_token = nullptr);
		
		bool expr(ExpressionPtr& out, Token* start_token = nullptr);
		bool assign(ExpressionPtr& out, Token* start_token = nullptr);
		bool addsub(ExpressionPtr& out, Token* start_token = nullptr);
		bool muldiv(ExpressionPtr& out, Token* start_token = nullptr);
		bool unary(ExpressionPtr& out, Token* starrt_token = nullptr);
		bool func_call(ExpressionPtr& out, Token* start_token = nullptr);
		bool paren(ExpressionPtr& out, Token* start_token = nullptr);
		bool array_init_list(ExpressionPtr& out, Token* start_token = nullptr);
		bool atom(ExpressionPtr& out, Token* start_token = nullptr);

		bool unary_plusminus(ExpressionPtr& out, Token* start_token = nullptr);
		bool unary_address(ExpressionPtr& out, Token* start_token = nullptr);
		bool number(ExpressionPtr& out, Token* start_token = nullptr);
		bool identifier(ExpressionPtr& out, Token* start_token = nullptr);

		bool type(TypePtr& out, Token* start_token = nullptr);
		bool array_type(TypePtr& out, Token* start_token = nullptr);
		bool reference_type(TypePtr& out, Token* start_token = nullptr);
		bool pointer_type(TypePtr& out, Token* start_token = nullptr);
		bool simple_type(TypePtr& out, Token* start_token = nullptr);

	private:
		TokenSeq token_seq_;
		TokenSeq::const_iterator token_iter_;
		AST ast_;

		Errors errors_;
		Warnings warnings_;
	};
}
