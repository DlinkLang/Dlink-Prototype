#pragma once

/**
 * @file Operation.hh
 * @date 2017.08.01
 * @author kmc7468, dev_kr
 * @brief Dlink 코드 파서가 생성하는 AST 노드들 중 연산과 관련된 노드들을 정의합니다.
 */

#include <cstdint>
#include <string>

#include "llvm/IR/Value.h"

#include "Root.hh"

namespace Dlink
{
	/**
	 * @brief int32_t 타입의 정수를 담는 파싱 노드입니다.
	 */
	struct Integer32 final : public Expression
	{
		/**
		 * @brief int32_t 타입의 정수 데이터입니다.
		 */
		std::int32_t data;

		/**
		 * @brief int32_t를 받아 멤버 필드 data를 초기화하고 Integer32를 생성합니다.
		 */
		Integer32(std::int32_t data_) : data(data_)
		{}

		std::string tree_gen(std::size_t depth) override;
		llvm::Value* code_gen() override;
	};
}

namespace Dlink
{
	/**
	 * @brief Dlink 연산자의 종류입니다.
	 */
	enum class Operator
	{
		None = 0,

		Plus, 			/**< 이항 덧셈, 단항 양의 부호 연산자입니다. */
		Minus, 			/**< 이항 뺄셈, 단항 음의 부호 연산자입니다. */
		Multiply, 		/**< 이항 곱셈 연산자입니다. */
		Divide, 		/**< 이항 나눗셈 연산자입니다. */
	};

	/**
	 * @brief 이항 연산의 구조를 담는 파싱 노드입니다.
	 */
	struct BinaryOperation final : public Expression
	{
		/**
		 * @brief 이항 연산의 연산자를 담는 Operator 타입의 멤버 필드입니다.
		 */
		Operator op;

		/**
		 * @brief 이항 연산의 좌측 피연산자를 담는 ExpressionPtr 타입의 멤버 필드입니다.
		 */
		ExpressionPtr lhs;
		/**
		 * @brief 이항 연산의 우측 피연산자를 담는 ExpressionPtr 타입의 멤버 필드입니다.
		 */
		ExpressionPtr rhs;

		/**
		 * @brief Operator와 ExpressionPtr 두 인수를 받아 멤버 필드 op, lhs, rhs를 초기화하고 BinaryOperation을 생성합니다.
		 */
		BinaryOperation(Operator op_, ExpressionPtr lhs_, ExpressionPtr rhs_)
			: op(op_), lhs(lhs_), rhs(rhs_)
		{}
	
		std::string tree_gen(std::size_t depth) override;
		llvm::Value* code_gen() override;
	};

	/**
	 * @brief 단항 연산의 구조를 담는 파싱 노드입니다.
	 */
	struct UnaryOperation final : public Expression
	{
		/*
		 * @brief 단항 연산의 연산자를 담는 Operator 타입의 멤버 필드입니다.
		 */
		Operator op;
		/**
		 * @brief 단항 연산의 피연산자를 담는 ExpressionPtr 타입의 멤버 필드입니다.
		 */
		ExpressionPtr rhs;

		/**
		 * @brief Operator와 ExpressionPtr를 받아 멤버 필드 op, rhs를 초기화하고 UnaryOperation을 생성합니다.
		 */
		UnaryOperation(Operator op_, ExpressionPtr rhs_)
			: op(op_), rhs(rhs_)
		{}
		
		std::string tree_gen(std::size_t depth) override;
		llvm::Value* code_gen() override;
	};
}
