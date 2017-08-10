#pragma once

/**
 * @file Parser.hh
 * @date 2017.08.10
 * @author dev_kr
 * @brief Parser 클래스를 정의합니다.
 */

#include <iostream>
#include <set>
#include <vector>

#include "Error.hh"
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
		bool parse(StatementPtr& output);
		const std::vector<Error>& get_errors() const noexcept;

	private:
		Token current_token() const;
		Token previous_token() const;
		Token next_token() const;
		bool accept(TokenType token_type);

		bool block(StatementPtr& out);
		bool scope(StatementPtr& out);
		bool var_decl(StatementPtr& out);
		bool func_decl(StatementPtr& out, TypePtr return_type, const std::string& identifier);
		bool expr_stmt(StatementPtr& out);
		
		bool expr(ExpressionPtr& out);
		bool assign(ExpressionPtr& out);
		bool addsub(ExpressionPtr& out);
		bool muldiv(ExpressionPtr& out);
		bool atom(ExpressionPtr& out);

		bool number(ExpressionPtr& out);
		bool identifier(ExpressionPtr& out);

		bool type(TypePtr& out);
		bool simple_type(TypePtr& out);

	private:
		TokenSeq input_;
		TokenSeq::const_iterator token_iter_;
		ExpressionPtr output_;

		Errors errors_;
	};
}
