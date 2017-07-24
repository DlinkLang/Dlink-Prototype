%{

#include <string>

#include "Scanner.hh"

#undef YY_DECL
#define YY_DECL int Dlink::Scanner::yylex(Dlink::Parser::semantic_type* const lval, Dlink::Parser::location_type* loc)

using token = Dlink::Parser::token;

#define YY_USER_ACTION loc->step(); loc->columns(yyleng);

%}

%option debug
%option nodefault
%option yyclass="Dlink::Scanner"
%option noyywrap
%option c++

%%

%{
	yylval = lval;
%}

[a-zA-Z]+ {
	yylval->build<std::string>(yytext);
	return(token::WORD);
}

\n {
	loc->lines();
	return(token::NEWLINE);
}

%%
