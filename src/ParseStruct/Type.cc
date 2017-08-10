#include "ParseStruct/Type.hh"
#include "CodeGen.hh"

namespace Dlink
{
	extern std::string tree_prefix(std::size_t depth);

	/**
	 * @brief 새 SimpleType 인스턴스를 만듭니다.
	 * @param identifier 타입의 식별자입니다.
	 */
	SimpleType::SimpleType(const std::string& identifier)
		: identifier(identifier)
	{}

	std::string SimpleType::tree_gen(std::size_t depth)
	{
		return tree_prefix(depth) + "SimpleType(" + identifier.id + ")\n";
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