#include "ParseStruct/Type.hh"
#include "CodeGen.hh"

namespace Dlink
{
	extern std::string tree_prefix(std::size_t depth);

	/**
	 * @brief 새 SimpleType 인스턴스를 만듭니다.
	 * @details is_unsigned가 false로 초기화 됩니다.
	 * @param identifier 타입의 식별자입니다.
	 */
	SimpleType::SimpleType(Identifier identifier)
		: SimpleType(identifier, false)
	{}
	/**
	 * @brief 새 SimpleType 인스턴스를 만듭니다.
	 * @param identifier 타입의 식별자입니다.
	 * @param is_unsigned 타입이 unsigned인지 여부입니다.
	 */
	SimpleType::SimpleType(Identifier identifier, bool is_unsigned)
		: identifier(identifier), is_unsigned(is_unsigned)
	{}

	std::string SimpleType::tree_gen(std::size_t depth) const
	{
		return tree_prefix(depth) + "SimpleType(" + identifier.id + ")\n"
			+ tree_prefix(depth + 1) + "is_unsigned: " + (is_unsigned ? "true" : "false");
	}
	llvm::Type* SimpleType::get_type()
	{
		if (identifier.id == "int")
		{
			return LLVM::builder.getInt32Ty();
		}

		return nullptr;
	}
}