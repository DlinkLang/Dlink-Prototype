#pragma once

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "parser.tab.hh"
#include "location.hh"

namespace Dlink
{
	class Scanner : public yyFlexLexer
	{
public:
		Scanner(std::istream* in) :	yyFlexLexer(in) {};
		virtual ~Scanner() {};

		using FlexLexer::yylex;

		virtual int yylex(Dlink::Parser::semantic_type * const lval, Dlink::Parser::location_type *location);

private:
		Dlink::Parser::semantic_type* yylval = nullptr;
	};
}
