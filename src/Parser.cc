#include "Parser.hh"

namespace Dlink
{
    const constexpr char Parser::grammar[] = R"(
                # Root Expression
                EXPR          <- BIN_ADDSUB

                # Binary Operations
                BIN_ADDSUB    <- _ BIN_MULDIV (ADDSUB_OP BIN_MULDIV)*
                BIN_MULDIV    <- ATOM (MULDIV_OP ATOM)*

                ATOM          <- NUMBER / '(' _ EXPR ')' _

                # Operators
                ADDSUB_OP     <- < [-+] > _
                MULDIV_OP     <- < [/*] > _

                # Literals
                NUMBER        <- < [0-9]+ > _

                # Ignoring Characters
                ~_            <- [ \t\r\n]*
    )";

    Parser::Parser()
    {
        auto BinaryOP_AST = [](const peg::SemanticValues& v) -> ExpressionPtr
        {    
            ExpressionPtr lhs = v[0].get<ExpressionPtr>();

            for (std::size_t i=1; i < v.size(); i += 2)
            {
                auto rhs = v[i+1].get<ExpressionPtr>();
                auto op = v[i].get<BinaryOperator>();

                lhs = std::make_shared<BinaryOP>(op, lhs, rhs);
            }
            
            return lhs;
        };
        
        load_grammar(grammar);
        
        auto parser = *this;

        parser["BIN_ADDSUB"]      = BinaryOP_AST;
        parser["BIN_MULDIV"]      = BinaryOP_AST;
        parser["ADDSUB_OP"]       = [](const peg::SemanticValues& v) -> BinaryOperator
        { 
            char op_ch = static_cast<char>(*v.c_str()); 
            
            if(op_ch == '-')
            {
                return BinaryOperator::Minus;
            }
            else if(op_ch == '+')
            {
                return BinaryOperator::Plus;
            }

            return BinaryOperator::None;
        };

        parser["MULDIV_OP"]      = [](const peg::SemanticValues& v) -> BinaryOperator
        { 
            char op_ch = static_cast<char>(*v.c_str()); 
            
            if(op_ch == '/')
            {
                return BinaryOperator::Divide;
            }
            else if(op_ch == '*')
            {
                return BinaryOperator::Multiply;
            }

            return BinaryOperator::None;
        };

        parser["NUMBER"]        = [](const peg::SemanticValues& v) -> ExpressionPtr
        { 
            int32_t data = std::atoi(v.c_str()); 
            ExpressionPtr node = std::make_shared<Integer32>(data);
            return node;
        };
    }
}
