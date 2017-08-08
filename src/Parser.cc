#include "Parser.hh"

namespace Dlink
{
    bool Parser::accept(TokenType token_type)
    {
        if((*current_token_).type == token_type)
        {
            current_token_++;
            return true;
        }

        return false;
    }
}

namespace Dlink
{
    Parser::Parser(const TokenSeq& input)
        : input_(input), current_token_(input_.cbegin())
    {}

    bool Parser::parse(ExpressionPtr& output)
    {
        return expr(output);
    }
   
    std::vector<Error> Parser::get_errors() const noexcept
    {
        return errors_.get_errors();
    }
}

namespace Dlink
{
    bool Parser::expr(ExpressionPtr& out)
    {
        return addsub(out);
    }

    bool Parser::addsub(ExpressionPtr& out)
    {
        ExpressionPtr lhs;

        if(!muldiv(lhs)) 
        {
            return false;
        }

        TokenType op;
       
        while(accept(TokenType::plus) || accept(TokenType::minus))
        {
            op = (*(current_token_ - 1)).type;
               
            ExpressionPtr rhs;
            if(!muldiv(rhs)) 
            {
                errors_.add_error(Error(*current_token_, "Expected expression, but got \"" + (*current_token_).data + "\""));
                return false;
            }

            lhs = std::make_shared<BinaryOperation>(op, lhs, rhs);
        }

        out = lhs; 
        return true;
    }
    
    bool Parser::muldiv(ExpressionPtr& out)
    {
        ExpressionPtr lhs;

        if(!number(lhs)) 
        {
            return false;
        }

        TokenType op;
       
        while(accept(TokenType::multiply) || accept(TokenType::divide))
        {
            op = (*(current_token_ - 1)).type;
               
            ExpressionPtr rhs;
            if(!number(rhs)) 
            {
                errors_.add_error(Error(*current_token_, "Expected expression, but got \"" + (*current_token_).data + "\""));
                return false;
            }

            lhs = std::make_shared<BinaryOperation>(op, lhs, rhs);
        }

        out = lhs; 
        return true;
    }

    bool Parser::number(ExpressionPtr& out)
    {
        if(accept(TokenType::dec_integer))
        {
            out = std::make_shared<Integer32>(std::stoi((*(current_token_ - 1)).data));
            return true;
        }
        
        return false;
    }
}
