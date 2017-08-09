#include "CodeGen.hh"
#include "ParseStruct.hh"

#include <map>
#include <string>
#include <utility>

#include "llvm/IR/Instructions.h"

namespace Dlink
{
	namespace LLVM
	{
		llvm::LLVMContext context;
		std::shared_ptr<llvm::Module> module = std::make_shared<llvm::Module>("top", context);
		llvm::IRBuilder<> builder(context);
	}

	static std::map<std::string, LLVM::Value> symbol;
}

namespace Dlink
{
	LLVM::Value Block::code_gen()
	{
		for (StatementPtr statement : statements)
		{
			statement->code_gen();
		}

		return nullptr;
	}

	LLVM::Value ExpressionStatement::code_gen()
	{
		return expression->code_gen();
	}
}

namespace Dlink
{
	LLVM::Value Integer32::code_gen()
	{
		return LLVM::builder.getInt32(data);
	}

	LLVM::Value BinaryOperation::code_gen()
	{
		LLVM::Value lhs_value = lhs->code_gen();
		LLVM::Value rhs_value = rhs->code_gen();

		switch (op)
		{
		case TokenType::plus:
			return LLVM::builder.CreateAdd(lhs_value, rhs_value);

		case TokenType::minus:
			return LLVM::builder.CreateSub(lhs_value, rhs_value);

		case TokenType::multiply:
			return LLVM::builder.CreateMul(lhs_value, rhs_value);

		case TokenType::divide:
			// TODO: 임시 방안
			return LLVM::builder.CreateSDiv(lhs_value, rhs_value);

		default:
			// TODO: 오류 처리
			return LLVM::builder.getFalse();
		}
	}

	LLVM::Value UnaryOperation::code_gen()
	{
		LLVM::Value rhs_value = rhs->code_gen();

		switch (op)
		{
		case TokenType::plus:
			return LLVM::builder.CreateMul(LLVM::builder.getInt32(1), rhs_value);

		case TokenType::minus:
			return LLVM::builder.CreateMul(LLVM::builder.getInt32(-1), rhs_value);

		default:
			// TODO: 오류 처리
			return LLVM::builder.getFalse();
		}
	}
}

namespace Dlink
{
	llvm::Type* SimpleType::get_type()
	{
		if (identifier == "int")
		{
			return LLVM::builder.getInt32Ty();
		}

		return nullptr;
	}
}

namespace Dlink
{
	LLVM::Value VariableDeclaration::code_gen()
	{
		llvm::AllocaInst* var = LLVM::builder.CreateAlloca(type->get_type(), nullptr, identifier);
		var->setAlignment(4);

		if (expression)
		{
			LLVM::Value init_expr = expression->code_gen();
			LLVM::builder.CreateStore(init_expr, var);
		}

		symbol.insert(std::pair<std::string, LLVM::Value>(identifier, var));
		return var;
	}
}
