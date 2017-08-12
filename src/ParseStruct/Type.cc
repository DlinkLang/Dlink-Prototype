#include "ParseStruct/Type.hh"
#include "CodeGen.hh"

namespace Dlink
{
	extern std::string tree_prefix(std::size_t depth);

	/**
	 * @brief 새 SimpleType 인스턴스를 만듭니다.
	 * @details is_unsigned가 false로 초기화 됩니다.
	 * @param token 이 노드를 만드는데 사용된 가장 첫번째 토큰입니다.
	 * @param identifier 타입의 식별자입니다.
	 */
	SimpleType::SimpleType(const Token& token, const std::string& identifier)
		: SimpleType(token, identifier, false)
	{}
	/**
	 * @brief 새 SimpleType 인스턴스를 만듭니다.
	 * @param token 이 노드를 만드는데 사용된 가장 첫번째 토큰입니다.
	 * @param identifier 타입의 식별자입니다.
	 * @param is_unsigned 타입이 unsigned인지 여부입니다.
	 */
	SimpleType::SimpleType(const Token& token, const std::string& identifier, bool is_unsigned)
		: Type(token), identifier(identifier), is_unsigned(is_unsigned)
	{}

	std::string SimpleType::tree_gen(std::size_t depth) const
	{
		return tree_prefix(depth) + "SimpleType(" + identifier + ")\n"
			+ tree_prefix(depth + 1) + "is_unsigned: " + (is_unsigned ? "true" : "false");
	}
	llvm::Type* SimpleType::get_type()
	{
		if (identifier == "int")
		{
			return LLVM::builder.getInt32Ty();
		}
		else if (identifier == "void")
		{
			return LLVM::builder.getVoidTy();
		}

		return nullptr;
	}

	/**
	 * @brief 새 StaticArray 인스턴스를 만듭니다.
	 * @param token 이 노드를 만드는데 사용된 가장 첫번째 토큰입니다.
	 * @param type 배열 아이템의 타입입니다.
	 * @param length 배열의 길이입니다.
	 */
	StaticArray::StaticArray(const Token& token, TypePtr type, ExpressionPtr length)
		: Type(token), type(type), length(length), expression(nullptr)
	{}
	/**
	 * @brief 새 StaticArray 인스턴스를 만듭니다.
	 * @param token 이 노드를 만드는데 사용된 가장 첫번째 토큰입니다.
	 * @param type 배열 아이템의 타입입니다.
	 * @param length 배열의 길이입니다.
	 * @param expression 배열 초기화 식입니다.
	 */
	StaticArray::StaticArray(const Token& token, TypePtr type, ExpressionPtr length, ExpressionPtr expression)
		: Type(token), type(type), length(length), expression(expression)
	{}
	std::string StaticArray::tree_gen(std::size_t depth) const
	{
		std::string result;
		result += tree_prefix(depth) + "StaticArray:\n";
		++depth;
		result += tree_prefix(depth) + "type:\n" + type->tree_gen(depth + 1) + '\n';
		result += tree_prefix(depth) + "length:\n" + length->tree_gen(depth + 1) + '\n';
		result += tree_prefix(depth) + "expression:";
		if (expression)
			result += " empty";
		else
			result += '\n' + expression->tree_gen(depth + 1);

		return result;
	}
	llvm::Type* StaticArray::get_type()
	{
		return nullptr;
	}
}
