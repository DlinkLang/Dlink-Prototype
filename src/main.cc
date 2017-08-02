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

    lexer.dump();

    return 0;
}
