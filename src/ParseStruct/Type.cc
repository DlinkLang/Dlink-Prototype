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
	LLVM::Type SimpleType::get_type()
	{
		if (identifier == "void")
		{
			return LLVM::builder().getVoidTy();
		}
		else if (identifier == "char")
		{
			return LLVM::builder().getInt8Ty();
		}
		else if (identifier == "byte")
		{
			return { LLVM::builder().getInt8Ty(), is_unsigned };
		}
		else if (identifier == "short")
		{
			return { LLVM::builder().getInt16Ty(), is_unsigned };
		}
		else if (identifier == "int")
		{
			return { LLVM::builder().getInt32Ty(), is_unsigned };
		}
		else if (identifier == "long")
		{
			return { LLVM::builder().getInt64Ty(), is_unsigned };
		}

		else if (identifier == "half")
		{
			return LLVM::builder().getHalfTy();
		}
		else if (identifier == "single")
		{
			return LLVM::builder().getFloatTy();
		}
		else if (identifier == "double")
		{
			return LLVM::builder().getDoubleTy();
		}

		return nullptr;
	}
	bool SimpleType::can_literal() const
	{
		return true;
	}

	/** 시작 토큰 값이 없는 미리 만들어진 void 타입입니다. */
	const TypePtr SimpleType::_void = std::make_shared<SimpleType>(Token::empty, "void");
	/** 시작 토큰 값이 없는 미리 만들어진 char 타입입니다. */
	const TypePtr SimpleType::_char = std::make_shared<SimpleType>(Token::empty, "char");
	/** 시작 토큰 값이 없는 미리 만들어진 byte 타입입니다. */
	const TypePtr SimpleType::byte = std::make_shared<SimpleType>(Token::empty, "byte", true);
	/** 시작 토큰 값이 없는 미리 만들어진 short 타입입니다. */
	const TypePtr SimpleType::_short = std::make_shared<SimpleType>(Token::empty, "short");
	/** 시작 토큰 값이 없는 미리 만들어진 int 타입입니다. */
	const TypePtr SimpleType::_int = std::make_shared<SimpleType>(Token::empty, "int");
	/** 시작 토큰 값이 없는 미리 만들어진 long 타입입니다. */
	const TypePtr SimpleType::_long = std::make_shared<SimpleType>(Token::empty, "long");

	/** 시작 토큰 값이 없는 미리 만들어진 signed byte 타입입니다. */
	const TypePtr SimpleType::signed_byte = std::make_shared<SimpleType>(Token::empty, "byte");
	/** 시작 토큰 값이 없는 미리 만들어진 unsigned short 타입입니다. */
	const TypePtr SimpleType::_unsigned_short = std::make_shared<SimpleType>(Token::empty, "short", true);
	/** 시작 토큰 값이 없는 미리 만들어진 unsigned int 타입입니다. */
	const TypePtr SimpleType::_unsigned_int = std::make_shared<SimpleType>(Token::empty, "int", true);
	/** 시작 토큰 값이 없는 미리 만들어진 unsigned long 타입입니다. */
	const TypePtr SimpleType::_unsigned_long = std::make_shared<SimpleType>(Token::empty, "long", true);

	/** 시작 토큰 값이 없는 미리 만들어진 half 타입입니다. */
	const TypePtr SimpleType::half = std::make_shared<SimpleType>(Token::empty, "half");
	/** 시작 토큰 값이 없는 미리 만들어진 single 타입입니다. */
	const TypePtr SimpleType::single = std::make_shared<SimpleType>(Token::empty, "single");
	/** 시작 토큰 값이 없는 미리 만들어진 double 타입입니다. */
	const TypePtr SimpleType::_double = std::make_shared<SimpleType>(Token::empty, "double");

	/** 
	 * @brief 새 ConstType 인스턴스를 만듭니다.
	 * @param token 이 노드를 만드는데 사용된 가장 첫번째 토큰입니다.
	 * @param type const 지정자가 없을 경우의 타입입니다.
	 */
	ConstType::ConstType(const Token& token, TypePtr type)
		: Type(token), type(type)
	{}
	std::string ConstType::tree_gen(std::size_t depth) const
	{
		return tree_prefix(depth) + "ConstType:\n" +
			tree_prefix(depth + 1) + "type:\n" +
			type->tree_gen(depth + 2);
	}
	LLVM::Type ConstType::get_type()
	{
		return { type->get_type(), true };
	}
	bool ConstType::can_literal() const
	{
		return type->can_literal();
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
	LLVM::Type StaticArray::get_type()
	{
		LLVM::Type type_llvm = type->get_type();
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
	bool StaticArray::can_literal() const
	{
		return type->can_literal();
	}

	/**
	 * @brief 이 Reference 인스턴스의 멤버를 초기화합니다.
	 * @param token 이 노드를 만드는데 사용된 가장 첫번째 토큰입니다.
	 * @param type 참조할 값의 타입입니다.
	 */
	Reference::Reference(const Token& token, TypePtr type)
		: Type(token), type(type)
	{}
	LLVM::Type Reference::get_type()
	{
		return type->get_type()->getPointerTo();
	}
	bool Reference::can_literal() const
	{
		return type->can_literal();
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
	LLVM::Type Pointer::get_type()
	{
		return type->get_type()->getPointerTo();
	}
	bool Pointer::is_safe() const noexcept
	{
		return false;
	}
	bool Pointer::can_literal() const
	{
		return true;
	}
}
