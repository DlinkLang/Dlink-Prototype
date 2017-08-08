#include <iostream>

#include "Lexer.hh"
#include "Parser.hh"

int main(int argc, const char** argv)
{
    Dlink::Lexer lexer;
    lexer.lex(R"(1 + 1 / 2 *)");
    
    std::cout << "Lexing Succeed\n";
    lexer.dump();
    std::cout << "\n";

    Dlink::TokenSeq token_seq = lexer.get_token_seq();
    
    Dlink::Parser parser(token_seq);
    
    Dlink::ExpressionPtr ast;
    if(parser.parse(ast))
    {
        std::cout << "Parsing Succeed\n";
        std::cout << ast->tree_gen(0) << '\n';
    }
    else
    {
        std::cout << "Parse Failed\n";
    }
    return 0;
}
