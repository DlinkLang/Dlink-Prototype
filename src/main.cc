#include <iostream>

#include "Lexer.hh"
#include "Parser.hh"

int main(int argc, const char** argv)
{
    Dlink::Lexer lexer;
    lexer.lex(R"(1 + 1 / 2 * 4)");
    
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

        for(auto error : parser.get_errors())
        {
            Dlink::Token error_token = error.error_token();
            std::cout << "Line " << error_token.line;
            std::cout << " Col " << error_token.col;

            std::cout << " " << error.what() << "\n";
        }
    }
    return 0;
}
