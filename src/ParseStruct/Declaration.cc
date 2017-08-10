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
		result += tree_prefix(depth) + "identifier: \n" + identifier.tree_gen(depth + 1) + "\n";
		result += tree_prefix(depth) + "type: \n" + type->tree_gen(depth + 1) + "\n";
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
}