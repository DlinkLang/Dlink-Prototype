#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <iostream>

namespace Dlink
{
	struct Node
	{
		/**
		 * @brief 현재 노드의 트리형 구조를 std::string 타입으로 시각화 시킵니다.
		 * @param 전체 트리에서 현재 노드의 깊이입니다.
		 * @return 현재 노드의 트리형 구조를 시각화 시킨 값을 반환합니다.
		 */
		virtual std::string treeGen(std::size_t depth) = 0;
	};

	struct Expression : public Node
	{};
}

namespace Dlink
{
	/**
	 * @brief Dlink의 이진 연산자의 열거 클래스 타입입니다.
	 */
	enum class BinaryOperator
	{
		None,
		Plus,
		Minus,
		Multiply,
		Divide,
	};

	using ExpressionPtr = std::shared_ptr<Expression>;
}

namespace Dlink
{
	/**
	 * @brief int32_t 타입의 정수를 담는 파싱 노드입니다.
	 */
	struct Integer32 final : public Expression
	{
		std::int32_t data;

		Integer32(std::int32_t data_) : data(data_)
		{}

		std::string treeGen(std::size_t depth) override;
	};

	/**
	 * @brief 이진 연산의 구조를 담는 파싱 노드입니다.
	 */
	struct BinaryOP final : public Expression
	{
		BinaryOperator op;
		ExpressionPtr lhs, rhs;

		BinaryOP(BinaryOperator op_, ExpressionPtr lhs_, ExpressionPtr rhs_)
			: op(op_), lhs(lhs_), rhs(rhs_)
		{}
		
		std::string treeGen(std::size_t depth) override;
	};
}
