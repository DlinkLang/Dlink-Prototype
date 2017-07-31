#include "CodeGen.hh"
#include "ParseStruct.hh"

namespace Dlink
{
	namespace LLVM
	{
		llvm::LLVMContext context;
		std::shared_ptr<llvm::Module> module = std::make_shared<llvm::Module>("top", context);
		llvm::IRBuilder<> builder(context);
	}
}

namespace Dlink
{
    llvm::Value* Block::code_gen()
    {
        for(StatementPtr statement : statements)
        {
            statement->code_gen();
        }

        return nullptr;
    }
    
    llvm::Value* ExpressionStatement::code_gen()
    {
        return expression->code_gen();
    }
}

namespace Dlink
{
	llvm::Value* Integer32::code_gen()
	{
		return LLVM::builder.getInt32(data);
	}
	
	llvm::Value* BinaryOperation::code_gen()
	{
		llvm::Value* lhs_value = lhs->code_gen();
		llvm::Value* rhs_value = rhs->code_gen();

		switch (op)
		{
		case Operator::Plus:
			return LLVM::builder.CreateAdd(lhs_value, rhs_value);

		case Operator::Minus:
			return LLVM::builder.CreateSub(lhs_value, rhs_value);

		case Operator::Multiply:
			return LLVM::builder.CreateMul(lhs_value, rhs_value);

		case Operator::Divide:
			// TODO: 임시 방안
			return LLVM::builder.CreateSDiv(lhs_value, rhs_value);

		default:
			// TODO: 오류 처리
			return LLVM::builder.getFalse();
		}
	}

	llvm::Value* UnaryOperation::code_gen()
	{
		llvm::Value* rhs_value = rhs->code_gen();

		switch (op)
		{
		case Operator::Plus:
			return LLVM::builder.CreateMul(LLVM::builder.getInt32(1), rhs_value);
		
		case Operator::Minus:
			return LLVM::builder.CreateMul(LLVM::builder.getInt32(-1), rhs_value);

		default:
			// TODO: 오류 처리
			return LLVM::builder.getFalse();
		}
	}
}
