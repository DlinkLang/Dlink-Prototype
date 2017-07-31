#include "Parser.hh"

namespace Dlink
{
    const constexpr char Parser::grammar[] = R"(
                BLOCK         <- (STMT)*

                STMT          <- EXPRSTMT
    
                EXPRSTMT      <- EXPR ';'

                # Root Expression
                EXPR          <- BIN_ADDSUB

                # Binary Operations
                BIN_ADDSUB    <- _ BIN_MULDIV (ADDSUB_OP BIN_MULDIV)*
                BIN_MULDIV    <- UNARY_SIGN (MULDIV_OP UNARY_SIGN)*
               
                # Unary Operations
                UNARY_SIGN    <- (ADDSUB_OP)? ATOM

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
        auto BinaryOperation_AST = [](const peg::SemanticValues& v) -> ExpressionPtr
        {    
            ExpressionPtr lhs = v[0].get<ExpressionPtr>();

            for (std::size_t i=1; i < v.size(); i += 2)
            {
                auto rhs = v[i+1].get<ExpressionPtr>();
                auto op = v[i].get<Operator>();

                lhs = std::make_shared<BinaryOperation>(op, lhs, rhs);
            }
            
            return lhs;
        };
        
        load_grammar(grammar); 
        auto parser = *this;
       
        parser["BLOCK"]           = [](const peg::SemanticValues& v) -> StatementPtr
        {
            std::vector<StatementPtr> statements;
            
            for(auto statement : v)
            {
                statements.push_back(statement.get<StatementPtr>());
            }

            StatementPtr result = std::make_shared<Block>(statements);
            return result;
        };

        parser["EXPRSTMT"]        = [](const peg::SemanticValues& v) -> StatementPtr
        {
            ExpressionPtr expression = v[0].get<ExpressionPtr>();
            StatementPtr result = std::make_shared<ExpressionStatement>(expression);
            return result;
        };

        parser["BIN_ADDSUB"]      = BinaryOperation_AST;
        parser["BIN_MULDIV"]      = BinaryOperation_AST;

        parser["UNARY_SIGN"]      = [](const peg::SemanticValues& v) -> ExpressionPtr
        {
            if(v.size() == 2)
            {
                Operator op = v[0].get<Operator>();
                ExpressionPtr rhs = v[1].get<ExpressionPtr>();
            
                ExpressionPtr result = std::make_shared<UnaryOperation>(op, rhs); 
                return result;
            }
            else
            {
                return v[0].get<ExpressionPtr>();
            }
        };

        parser["NUMBER"]          = [](const peg::SemanticValues& v) -> ExpressionPtr
        { 
            int32_t data = std::atoi(v.c_str()); 
            ExpressionPtr result = std::make_shared<Integer32>(data);
            return result;
        };

        parser["ADDSUB_OP"]       = [](const peg::SemanticValues& v) -> Operator
        { 
            char op_ch = static_cast<char>(*v.c_str()); 
            
            if(op_ch == '-')
            {
                return Operator::Minus;
            }
            else if(op_ch == '+')
            {
                return Operator::Plus;
            }

            return Operator::None;
        };

        parser["MULDIV_OP"]       = [](const peg::SemanticValues& v) -> Operator
        { 
            char op_ch = static_cast<char>(*v.c_str()); 
            
            if(op_ch == '/')
            {
                return Operator::Divide;
            }
            else if(op_ch == '*')
            {
                return Operator::Multiply;
            }

            return Operator::None;
        };
    }
}
