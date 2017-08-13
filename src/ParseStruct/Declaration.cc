#include "ParseStruct/Declaration.hh"
#include "ParseStruct/Type.hh"
#include "CodeGen.hh"

namespace Dlink
{
	extern std::string tree_prefix(std::size_t depth);

	/**
	 * @brief 새 VariableDeclaration 인스턴스를 만듭니다.
	 * @param token 이 노드를 만드는데 사용된 가장 첫번째 토큰입니다.
	 * @param type 변수의 타입입니다.
	 * @param identifier 변수의 식별자입니다.
	 */
	VariableDeclaration::VariableDeclaration(const Token& token, TypePtr type,
		const std::string& identifier)
		: Statement(token), type(type), identifier(identifier)
	{}
	/**
	* @brief 새 VariableDeclaration 인스턴스를 만듭니다.
	* @param token 이 노드를 만드는데 사용된 가장 첫번째 토큰입니다.
	* @param type 변수의 타입입니다.
	* @param identifier 변수의 식별자입니다.
	* @param expression 변수의 초기화 식입니다.
	*/
	VariableDeclaration::VariableDeclaration(const Token& token, TypePtr type,
		const std::string& identifier, ExpressionPtr expression)
		: Statement(token), type(type), identifier(identifier), expression(expression)
	{}

	std::string VariableDeclaration::tree_gen(std::size_t depth) const
	{
		std::string result;
		result += tree_prefix(depth) + "VariableDeclaration:\n";
		++depth;
		result += tree_prefix(depth) + "type:\n" + type->tree_gen(depth + 1) + '\n';
		result += tree_prefix(depth) + "identifier: " + identifier + '\n';
		if (expression)
			result += tree_prefix(depth) + "expression: \n" + expression->tree_gen(depth + 1);
		else
			result += tree_prefix(depth) + "expression: empty";

		return result;
	}
	LLVM::Value VariableDeclaration::code_gen()
	{
		llvm::AllocaInst* var = LLVM::builder.CreateAlloca(type->get_type(), nullptr, identifier);
		var->setAlignment(4);

		if (dynamic_cast<LValueReference*>(type.get()))
		{
			if (!expression)
			{
				// TODO: 에러 메세지 추가해 주세요.
				// TODO: int& i; 이런식으로 초기화 식이 없는 상황입니다.
				throw Error(token, "TODO");
			}
			else
			{
				// TODO: 좌측 값 참조 초기화 식 대입(?)
				return nullptr;
			}
		}
		else if (expression) // Reference가 아닌데 expression이 있는 상황
		{
			LLVM::Value init_expr = expression->code_gen();
			
			LLVM::builder.CreateStore(init_expr, var);
		}

		symbol_table->map.insert(std::make_pair(identifier, var));
		return var;
	}

	/**
	 * @brief 새 FunctionDeclaration 인스턴스를 만듭니다.
	 * @param return_type 함수의 반환 값 타입입니다.
	 * @param identifier 함수의 식별자입니다.
	 * @param parameter 함수의 매개 변수입니다.
	 * @param body 함수의 몸체입니다.
	 */
	FunctionDeclaration::FunctionDeclaration(const Token& token, TypePtr return_type, const std::string& identifier,
		const std::vector<VariableDeclaration>& parameter, StatementPtr body)
		: Statement(token), return_type(return_type), identifier(identifier), parameter(parameter), body(body)
	{
		std::vector<llvm::Type*> param_type;
		for (const auto& param : parameter)
		{
			param_type.push_back(param.type->get_type());
		}

		func_type_ =
			param_type.size() != 0 ?
			llvm::FunctionType::get(return_type->get_type(), param_type, false) :
			llvm::FunctionType::get(return_type->get_type(), false);
		func_ =
			llvm::Function::Create(func_type_, llvm::GlobalValue::ExternalLinkage, identifier, LLVM::module.get());

		std::size_t i = 0;
		for (auto& param : func_->args())
		{
			param.setName(parameter[i++].identifier);
		}

		symbol_table->map.insert(std::make_pair(identifier, func_));
	}
	/**
	 * @brief 새 더미 FunctionDeclaration 인스턴스를 만듭니다.
	 * @details 마지막 bool 타입 인수로는 아무 값이나 주어도 더미 인스턴스로 만들어집니다.
	 * @param return_type 함수의 반환 값 타입입니다.
	 * @param identifier 함수의 식별자입니다.
	 * @param parameter 함수의 매개 변수입니다.
	 * @param body 함수의 몸체입니다.
	 */
	FunctionDeclaration::FunctionDeclaration(const Token& token, TypePtr return_type, const std::string& identifier,
		const std::vector<VariableDeclaration>& parameter, StatementPtr body, bool)
		: Statement(token), return_type(return_type), identifier(identifier), parameter(parameter), body(body),
		func_(nullptr), func_type_(nullptr)
	{}

	std::string FunctionDeclaration::tree_gen(std::size_t depth) const
	{
		std::string result;
		result += tree_prefix(depth) + "FunctionDeclaration:\n";
		++depth;
		result += tree_prefix(depth) + "return_type:\n" + return_type->tree_gen(depth + 1) + '\n';
		result += tree_prefix(depth) + "identifier: " + identifier + '\n';
		result += tree_prefix(depth) + "parameter:";
		if (parameter.size() == 0)
			result += " empty\n";
		else
		{
			result += '\n';

			++depth;

			for (auto& param : parameter)
			{
				result += param.tree_gen(depth) + '\n';
			}

			--depth;
		}
		result += tree_prefix(depth) + "body:\n" + body->tree_gen(depth + 1);

		return result;
	}
	LLVM::Value FunctionDeclaration::code_gen()
	{
		current_func = std::make_shared<FunctionDeclaration>(token, return_type, identifier, parameter, body, true);
		
		llvm::BasicBlock* func_block = llvm::BasicBlock::Create(LLVM::context, "entry", func_, nullptr);
		LLVM::builder.SetInsertPoint(func_block);

		for(auto& param : func_->args())
		{
			llvm::AllocaInst* param_alloca = LLVM::builder.CreateAlloca(param.getType(), nullptr, param.getName());
			LLVM::builder.CreateStore(&param, param_alloca);

			symbol_table->map.insert(std::make_pair(param.getName(), param_alloca));
		}

		llvm::Value* body_gen = body->code_gen();
		llvm::ReturnInst* ret = nullptr;

		if (body_gen)
		{
			ret = llvm::dyn_cast<llvm::ReturnInst>(body_gen);
		}

		if (!ret)
		{
			if (LLVM::builder.getCurrentFunctionReturnType() != LLVM::builder.getVoidTy())
			{
				LLVM::builder.CreateRet(llvm::Constant::getNullValue(LLVM::builder.getCurrentFunctionReturnType()));
				
				CompileMessage::warnings.add_warning(Warning(token, "Expected return statement at the end of non-void returning function declaration; null value will be returned"));
				// throw Error(token, "Expected return statement at the end of non-void returning function declaration");
			}
			else
			{
				// 함수의 반환 값 타입이 void라면 void를 리턴합니다.
				LLVM::builder.CreateRetVoid();
			}
		}

		LLVM::function_pm->run(*func_);
		
		for (auto& param : func_->args())
		{
			symbol_table->map.erase(param.getName());
		}

		current_func = nullptr;

		return func_;
	}
}
