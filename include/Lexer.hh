#pragma once

/**
 * @file Driver.hh
 * @date 2017.7.24
 * @author dev_kr
 * @brief Flex로 생성된 yyFlexLexer를 추상화시킨 Lexer 클래스를 정의합니다.
 */

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "parser.tab.hh"
#include "location.hh"

namespace Dlink
{	
	/**
	 * @brief Flex로 생성된 렉서를 추상화 시킨 클래스입니다.
	 */
	class Lexer : public yyFlexLexer
	{
	public:
		Lexer(std::istream* in) : yyFlexLexer(in) 
		{};
		virtual ~Lexer() 
		{};

		using FlexLexer::yylex;

		/**
		 * @brief 렉싱 과정에 호출되는 함수입니다.
		 * @param lval 렉싱 과정에서 현재 토큰의 값입니다.
		 * @param location 렉싱 과정에서 현재 토큰의 위치입니다.
		 */
		virtual int yylex(Dlink::Parser::semantic_type* const lval, Dlink::Parser::location_type* location);
	private:
		Dlink::Parser::semantic_type* yylval = nullptr;
	};
}
