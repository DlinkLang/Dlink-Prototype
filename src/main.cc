#include <iostream>

#include "Lexer.hh"

int main(int argc, const char** argv)
{
    Dlink::Lexer lexer;
    lexer.lex(R"(
int main()
{
    int a = a++;
    return 0;
}
    )");

    auto result = lexer.get_token_seq();

    for(auto tok : result)
    {
        std::cout << "Line " << tok.line << " Col " << tok.col << " " << tok.data << "\n";
    }

    return 0;
}
