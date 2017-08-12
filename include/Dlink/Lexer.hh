#pragma once

/**
 * @file Lexer.hh
 * @date 2017.08.02
 * @author dev_kr
 * @brief Lexer 클래스를 정의합니다.
 */

#include <iostream>
#include <map>
#include <sstream>
#include <vector>

#include "Token.hh"

namespace Dlink
{
	/**
	 * @brief Dlink 코드를 토큰 단위로 쪼개는 렉서입니다.
	 * @details 이 클래스는 다른 곳에서 상속받을 수 없습니다.
	 * @see Dlink::Token
	 */
	class Lexer final
	{
	public:
		Lexer();
		Lexer(const Lexer& lexer) = delete;
		Lexer(Lexer&& lexer) noexcept = delete;
		~Lexer() = default;

	public:
		Lexer& operator=(const Lexer& lexer) = delete;
		Lexer& operator=(Lexer&& lexer) noexcept = delete;
		bool operator==(const Lexer& lexer) const noexcept = delete;
		bool operator!=(const Lexer& lexer) const noexcept = delete;

	public:
		void lex(const std::string& code);
		const TokenSeq& get_token_seq() const noexcept;
		void dump() const;
		void dump(std::ostream& out) const;

	private:
		TokenSeq token_seq_;
		std::map<std::string, TokenType> keyword_map_;
	};
}
