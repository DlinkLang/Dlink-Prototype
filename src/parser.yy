%skeleton "lalr1.cc"
%require  "3.0"
%debug 
%defines 
%define api.namespace {Dlink}
%define parser_class_name {Parser}

%code requires{
   namespace Dlink 
   {
   		 class Driver;
		 class Lexer;
   }

}

%parse-param { Lexer& lexer }
%parse-param { Driver& driver }

%code{
   #include <iostream>
   #include <cstdlib>
   #include <fstream>
	
   #include "Driver.hh"

   #undef yylex
   #define yylex lexer.yylex
}

%define api.value.type variant
%define parse.assert

%token               END    0     "end of file"
%token <std::string> IDENTIFIER
%token <int>		 INTEGER
%token 				 NEWLINE

%locations

%%

list_option : END | list END;

list
  : item
  | list item
	;

item
  : IDENTIFIER    { std::cout << "WORD\n"; 	}
  |	INTEGER 	  { std::cout << "INTEGER\n";  }
  | NEWLINE 	  { std::cout << "NEWLINE\n"; }
	;

%%
void Dlink::Parser::error( const location_type &l, const std::string &err_message )
{
     std::cerr << "Error: " << err_message << " at " << l << "\n";
}
