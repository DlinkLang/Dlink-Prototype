#include "ParseStruct/Declaration.hh"
#include "CodeGen.hh"

namespace Dlink
{
	extern std::string tree_prefix(std::size_t depth);

	/**
	 * @brief 새 VariableDeclaration 인스턴스를 만듭니다.
	 * @param type 변수의 타입입니다.
	 * @param identifier 변수의 식별자입니다.
	 */
	VariableDeclaration::VariableDeclaration(TypePtr type, const std::string& identifier)
		: type(type), identifier(identifier)
	{}
	/**
	* @brief 새 VariableDeclaration 인스턴스를 만듭니다.
	* @param type 변수의 타입입니다.
	* @param identifier 변수의 식별자입니다.
	* @param expression 변수의 초기화 식입니다.
	*/
	VariableDeclaration::VariableDeclaration(TypePtr type, const std::string& identifier,
		ExpressionPtr expression)
		: type(type), identifier(identifier), expression(expression)
	{}

	std::string VariableDeclaration::tree_gen(std::size_t depth)
	{
		std::string result;
		result += tree_prefix(depth) + "VariableDeclaration:\n";
		++depth;
		result += tree_prefix(depth) + "type: \n" + type->tree_gen(depth + 1) + "\n";
		result += tree_prefix(depth) + "identifier: \n" + identifier.tree_gen(depth + 1) + "\n";
		if (expression)
			result += tree_prefix(depth) + "expression: \n" + expression->tree_gen(depth + 1) + "\n";
		else
			result += tree_prefix(depth) + "expression: empty\n";

		return result;
	}
	LLVM::Value VariableDeclaration::code_gen()
	{
		llvm::AllocaInst* var = LLVM::builder.CreateAlloca(type->get_type(), nullptr, identifier.id);
		var->setAlignment(4);

		if (expression)
		{
			LLVM::Value init_expr = expression->code_gen();
			LLVM::builder.CreateStore(init_expr, var);
		}

		symbol_table->map.insert(std::make_pair(identifier.id, var));
		return var;
	}

	FunctionDeclaration::FunctionDeclaration(TypePtr return_type, Identifer identifier,
		const std::vector<VariableDeclaration>& parameter, StatementPtr body)
		: return_type(return_type), identifier(identifier), parameter(parameter), body(body)
	{}
	std::string FunctionDeclaration::tree_gen(std::size_t depth)
	{
		std::string result;
		result += tree_prefix(depth) + "FunctionDeclaration:\n";
		++depth;
		result += tree_prefix(depth) + "return_type:\n" + return_type->tree_gen(depth + 1) + '\n';
		result += tree_prefix(depth) + "identifier:\n" + identifier.tree_gen(depth + 1) + '\n';
		result += tree_prefix(depth) + "body:\n" + body->tree_gen(depth + 1) + '\n';

		return result;
	}
	LLVM::Value FunctionDeclaration::code_gen()
	{
		std::vector<llvm::Type*> param_type;
		for (const auto& param : parameter)
		{
			param_type.push_back(param.type->get_type());
		}

		llvm::FunctionType* func_type = llvm::FunctionType::get(return_type->get_type(), param_type, false);
		llvm::Function* func =
			llvm::Function::Create(func_type, llvm::GlobalValue::ExternalLinkage, identifier.id, LLVM::module.get());

		std::size_t i = 0;
		for (auto& param : func->args())
		{
			param.setName(parameter[i++].identifier.id);
		}

		llvm::BasicBlock* func_block = llvm::BasicBlock::Create(LLVM::context, "entry", func, nullptr);
		LLVM::builder.SetInsertPoint(func_block);

		auto new_sym_table = std::make_shared<SymbolTable>();
		new_sym_table->parent = symbol_table;
		symbol_table = new_sym_table;

		for (auto& arg : func->args())
		{
			llvm::AllocaInst* alloca_inst = LLVM::builder.CreateAlloca(arg.getType());
			LLVM::builder.CreateStore(&arg, alloca_inst);
			symbol_table->map[arg.getName()] = alloca_inst;
		}

		llvm::Value* body_gen = body->code_gen();
		llvm::ReturnInst* ret = nullptr;

		symbol_table = symbol_table->parent;

		if (body_gen)
		{
			ret = llvm::dyn_cast<llvm::ReturnInst>(body_gen);
		}

		if (!ret)
		{
			if (LLVM::builder.getCurrentFunctionReturnType() != LLVM::builder.getVoidTy())
			{
				// 함수의 반환 값 타입이 void는 아니지만 반환 값이 없을 경우 null을 리턴합니다.
				LLVM::builder.CreateRet(llvm::Constant::getNullValue(LLVM::builder.getCurrentFunctionReturnType()));
			}
			else
			{
				// 함수의 반환 값 타입이 void라면 void를 리턴합니다.
				LLVM::builder.CreateRetVoid();
			}
		}

		LLVM::function_pm->run(*func);

		return func;
	}
}