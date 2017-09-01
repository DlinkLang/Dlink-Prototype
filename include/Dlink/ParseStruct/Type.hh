#pragma once

/**
 * @file Type.hh
 * @author dev_kr, kmc7468
 * @brief Dlink 코드 파서의 결과가 생성하는 추상 구문 트리 노드들 중 타입과 관련된 노드들을 정의합니다.
 */

#include <string>

#include "llvm/IR/Type.h"

#include "Root.hh"
#include "../LLVMType.hh"

namespace Dlink
{
	/**
	 * @brief 한정자 등이 없는 순수한 타입입니다.
	 * @details 이 구조체는 다른 곳에서 상속받을 수 없습니다.
	 */
	struct SimpleType final : public Type
	{
		SimpleType(const Token& token, const std::string& identifier);
		SimpleType(const Token& token, const std::string& identifier, bool is_unsigned);

		std::string tree_gen(std::size_t depth) const override;
		LLVM::Type get_type() override;

		/** 타입의 식별자입니다. */
		const std::string identifier;
		/**
		 * @brief 타입이 unsigned인지 여부입니다.
		 * @details 만약 타입이 unsigned라면 true, signed이거나 unsigned/signed 구분이 필요 없는 경우에는 false입니다.
		 */
		const bool is_unsigned;
	};

	/**
	 * @brief const 지정자가 있는 타입입니다.
	 */
	struct ConstType final : public Type
	{
		ConstType(const Token& token, TypePtr type);

		std::string tree_gen(std::size_t depth) const override;
		LLVM::Type get_type() override;

		/** const 지정자가 없을 경우의 타입입니다. */
		TypePtr type;
	};

	/**
	 * @brief 정적 배열 타입입니다.
	 * @details 이 구조체는 다른 곳에서 상속받을 수 없습니다.
	 */
	struct StaticArray final : public Type
	{
		StaticArray(const Token& token, TypePtr type, ExpressionPtr length);

		std::string tree_gen(std::size_t depth) const override;
		LLVM::Type get_type() override;

		/** 배열 아이템의 타입입니다. */
		TypePtr type;
		/** 배열의 길이입니다. */
		ExpressionPtr length;
	};

	/**
	 * @brief 참조 타입의 루트 노드입니다.
	 */
	struct Reference : public Type
	{
		Reference(const Token& token, TypePtr type);

		LLVM::Type get_type() override;

		/** 참조하고 있는 값의 타입입니다. */
		TypePtr type;
	};

	/**
	 * @brief 좌측 값 참조 타입입니다.
	 * @details 이 구조체는 다른 곳에서 상속받을 수 없습니다.
	 */
	struct LValueReference final : public Reference
	{
		using Reference::Reference;

		std::string tree_gen(std::size_t depth) const override;
	};

	/**
	 * @brief 포인터 타입입니다.
	 * @details 이 구조체는 다른 곳에서 상속받을 수 없습니다.
	 */
	struct Pointer final : public Type
	{
		Pointer(const Token& token, TypePtr type);

		std::string tree_gen(std::size_t depth) const override;
		LLVM::Type get_type() override;
		bool is_safe() const noexcept override;

		/** 포인터의 원본 타입입니다. */
		TypePtr type;
	};
}
