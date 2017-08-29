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
			return LLVM::builder().getInt32Ty();
		}
		else if (identifier == "void")
		{
			return LLVM::builder().getVoidTy();
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
		: Type(token), type(type), length(length)
	{}
	std::string StaticArray::tree_gen(std::size_t depth) const
	{
		std::string result;
		result += tree_prefix(depth) + "StaticArray:\n";
		++depth;
		result += tree_prefix(depth) + "type:\n" + type->tree_gen(depth + 1) + '\n';
		result += tree_prefix(depth) + "length:\n" + length->tree_gen(depth + 1);

		return result;
	}
	llvm::Type* StaticArray::get_type()
	{
		llvm::Type* type_llvm = type->get_type();
		std::uint64_t length_real = 0;
		Any length_any;
		bool length_ok = length->evaluate(length_any);

		if (!length_ok)
		{
			throw Error(token, "Expected compile time integral value");
		}

		if (length_any.type() == typeid(std::int64_t))
		{
			std::int64_t length_any_get = length_any.get<std::int64_t>();

			if (length_any_get < 0)
			{
				throw Error(token, "Expected positive integral value");
			}

			length_real = length_any_get;
		}
		else if (length_any.type() == typeid(std::uint64_t))
		{
			length_real = length_any.get<std::uint64_t>();
		}

		return llvm::ArrayType::get(type_llvm, length_real);
	}

	/**
	 * @brief 이 Reference 인스턴스의 멤버를 초기화합니다.
	 * @param token 이 노드를 만드는데 사용된 가장 첫번째 토큰입니다.
	 * @param type 참조할 값의 타입입니다.
	 */
	Reference::Reference(const Token& token, TypePtr type)
		: Type(token), type(type)
	{}
	llvm::Type* Reference::get_type()
	{
		return type->get_type()->getPointerTo();
	}

	std::string LValueReference::tree_gen(std::size_t depth) const
	{
		return tree_prefix(depth) + "LValueReference:\n" +
			tree_prefix(depth + 1) + "type:\n" + type->tree_gen(depth + 1);
	}

	/**
	 * @brief 새 Pointer 인스턴스를 만듭니다.
	 * @param token 이 노드를 만드는데 사용된 가장 첫번째 토큰입니다.
	 * @param type 포인터의 원본 타입입니다.
	 */
	Pointer::Pointer(const Token& token, TypePtr type)
		: Type(token), type(type)
	{}
	std::string Pointer::tree_gen(std::size_t depth) const
	{
		return tree_prefix(depth) + "PointerType:\n" +
			type->tree_gen(depth + 1);
	}
	llvm::Type* Pointer::get_type()
	{
		return type->get_type()->getPointerTo();
	}
	bool Pointer::is_safe() const noexcept
	{
		return false;
	}
}
