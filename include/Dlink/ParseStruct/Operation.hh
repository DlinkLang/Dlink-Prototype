#pragma once

/**
 * @file Operation.hh
 * @author kmc7468, dev_kr
 * @brief Dlink 코드 파서의 결과가 생성하는 추상 구문 트리의 노드들 중 연산과 관련된 노드들을 정의합니다.
 */

#include <cstdint>
#include <string>

#include "llvm/IR/Value.h"

#include "../LLVMValue.hh"
#include "../Token.hh"
#include "../ParseStruct/Root.hh"
#include "../Any.hh"

namespace Dlink
{
	/**
	 * @brief 32비트 부호 있는 정수 상수를 저장하는 추상 구문 트리의 노드입니다.
	 * @details 이 구조체는 다른 곳에서 상속받을 수 없습니다.
	 */
	struct Integer32 final : public Expression
	{
		Integer32(const Token& token, std::int32_t data) noexcept;

		std::string tree_gen(std::size_t depth) const override;
		LLVM::Value code_gen() override;
		bool evaluate(Any& out) override;

		/** 32비트 부호 있는 정수 상수입니다. */
		std::int32_t data;
	};

	/**
	 * @brief 문자열을 저장하는 추상 구문 트리의 노드입니다.
	 * @details 이 구조체는 다른 곳에서 상속받을 수 없습니다.
	 */
	struct String final : public Expression
	{
		String(const Token& token, const std::string& data) noexcept;

		std::string tree_gen(std::size_t depth) const override;
		LLVM::Value code_gen() override;

		/** 문자열입니다. */
		std::string data;
	};

	/**
	 * @brief 문자를 저장하는 추상 구문 트리의 노드입니다.
	 * @details 이 구조체는 다른 곳에서 상속받을 수 없습니다.
	 */
	struct Character final : public Expression
	{
		Character(const Token& token, char data) noexcept;

		std::string tree_gen(std::size_t depth) const override;
		LLVM::Value code_gen() override;

		/** 문자입니다. */
		char data;
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
		BinaryOperation(const Token& token, TokenType op, ExpressionPtr lhs, ExpressionPtr rhs);

		std::string tree_gen(std::size_t depth) const override;
		LLVM::Value code_gen() override;
		void preprocess() override;
		bool evaluate(Any& out) override;

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
		UnaryOperation(const Token& token, TokenType op, ExpressionPtr rhs);

		std::string tree_gen(std::size_t depth) const override;
		LLVM::Value code_gen() override;
		void preprocess() override;
		bool evaluate(Any& out) override;

		/** 연산자 타입입니다. */
		TokenType op;
		/** 피연산자입니다. */
		ExpressionPtr rhs;
	};

	/**
	 * @brief 함수 호출 연산의 구조를 담는 추상 구문 트리의 노드입니다.
	 * @details 이 구조체는 다른 곳에서 상속받을 수 없습니다.
	 */
	struct FunctionCallOperation final : public Expression
	{
		FunctionCallOperation(const Token& token, ExpressionPtr func_expr, const std::vector<ExpressionPtr>& arugment);

		std::string tree_gen(std::size_t depth) const override;
		LLVM::Value code_gen() override;
		void preprocess() override;

		/** 호출할 함수의 식입니다. */
		ExpressionPtr func_expr;
		/** 인수입니다. */
		std::vector<ExpressionPtr> argument;
	};

	/**
	 * @brief 배열 초기화 리스트의 구조를 담는 추상 구문 트리의 노드입니다.
	 * @details 이 구조체는 다른 곳에서 상속받을 수 없습니다.
	 */
	struct ArrayInitList final : public Expression
	{
		ArrayInitList(const Token& token, const std::vector<ExpressionPtr>& elements);

		std::string tree_gen(std::size_t depth) const override;
		LLVM::Value code_gen() override;
		void preprocess() override;

		/** 배열 리스트의 원소들입니다. */
		std::vector<ExpressionPtr> elements;
	};

	/**
	 * @brief 안전하지 않은 식을 담는 추상 구문 트리의 노드입니다.
	 * @details 이 구조체는 다른 곳에서 상속받을 수 없습니다.
	 */
	struct UnsafeExpression final : public Expression
	{
		UnsafeExpression(const Token& token, ExpressionPtr expression);

		std::string tree_gen(std::size_t depth) const override;
		LLVM::Value code_gen() override;
		void preprocess() override;

		/** 안전하지 않은 식입니다. */
		ExpressionPtr expression;
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
		ReturnStatement(const Token& token, ExpressionPtr return_value = nullptr);

		std::string tree_gen(std::size_t depth) const override;
		LLVM::Value code_gen() override;
		void preprocess() override;

		/** 반환할 식입니다. */
		ExpressionPtr return_expr;
	};

	/**
	* @brief 안전하지 않은 문을 담는 추상 구문 트리의 노드입니다.
	* @details 이 구조체는 다른 곳에서 상속받을 수 없습니다.
	*/
	struct UnsafeStatement final : public Statement
	{
		UnsafeStatement(const Token& token, StatementPtr statement);

		std::string tree_gen(std::size_t depth) const override;
		LLVM::Value code_gen() override;
		void preprocess() override;

		/** 안전하지 않은 문입니다. */
		StatementPtr statement;
	};
}