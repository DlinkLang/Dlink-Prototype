#pragma once

/**
 * @file Operation.hh
 * @date 2017.08.01
 * @author kmc7468, dev_kr
 * @brief Dlink 코드 파서의 결과가 생성하는 추상 구문 트리의 노드들 중 연산과 관련된 노드들을 정의합니다.
 */

#include <cstdint>
#include <string>

#include "llvm/IR/Value.h"

#include "Root.hh"
#include "../LLVMValue.hh"
#include "../Token.hh"

namespace Dlink
{
	/**
	 * @brief 32비트 부호 있는 정수 상수를 저장하는 추상 구문 트리의 노드입니다.
	 * @details 이 구조체는 다른 곳에서 상속받을 수 없습니다.
	 */
	struct Integer32 final : public Expression
	{
		Integer32(std::int32_t data_) noexcept;

		std::string tree_gen(std::size_t depth) override;
		LLVM::Value code_gen() override;

		/** 32비트 부호 있는 정수 상수입니다. */
		std::int32_t data;
	};
}

namespace Dlink
{
	/**
	 * @brief 이항 연산의 구조를 담는 추상 구문 트리의 노드입니다.
	 * @details 이 구조체는 다른 곳에서 상속받을 수 없습니다.
	 */
	struct BinaryOperation final : public Expression
	{
		BinaryOperation(TokenType op_, ExpressionPtr lhs_, ExpressionPtr rhs_);

		std::string tree_gen(std::size_t depth) override;
		LLVM::Value code_gen() override;

		/** 연산자 타입입니다. */
		TokenType op;
		/** 이항 연산에서의 좌측 피연산자입니다. */
		ExpressionPtr lhs;
		/** 이항 연산에서의 우측 피연산자입니다. */
		ExpressionPtr rhs;
	};

	/**
	 * @brief 단항 연산의 구조를 담는 추상 구문 트리의 노드입니다.
	 * @details 이 구조체는 다른 곳에서 상속받을 수 없습니다.
	 */
	struct UnaryOperation final : public Expression
	{
		UnaryOperation(TokenType op_, ExpressionPtr rhs_);

		std::string tree_gen(std::size_t depth) override;
		LLVM::Value code_gen() override;

		/** 연산자 타입입니다. */
		TokenType op;
		/** 피연산자입니다. */
		ExpressionPtr rhs;
	};
}

namespace Dlink
{
	/**
	 * @brief return 문을 담는 추상 구문 트리의 노드입니다.
	 * @details 이 구조체는 다른 곳에서 상속받을 수 없습니다.
	 */
	struct ReturnStatement final : public Statement
	{
		ReturnStatement(ExpressionPtr return_value);

		std::string tree_gen(std::size_t depth) override;
		LLVM::Value code_gen() override;

		/** 반환할 식입니다. */
		ExpressionPtr return_expr;
	};
}