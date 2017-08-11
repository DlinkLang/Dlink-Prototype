#pragma once

/**
 * @file Declaration.hh
 * @date 2017.08.11
 * @author kmc7468
 * @brief Dlink 코드 파서의 결과가 생성하는 추상 구문 트리의 노드들 중 선언과 관련된 노드들을 정의합니다.
 */

#include <memory>
#include <string>

#include "llvm/IR/Value.h"

#include "Root.hh"
#include "../LLVMValue.hh"

namespace Dlink
{
	/**
	 * @brief 변수 선언문의 구조를 담는 추상 구문 트리의 노드입니다.
	 */
	struct VariableDeclaration : public Statement
	{
		VariableDeclaration(TypePtr type, const std::string& identifier);
		VariableDeclaration(TypePtr type, const std::string& identifier, ExpressionPtr expression);

		std::string tree_gen(std::size_t depth) const override;
		LLVM::Value code_gen() override;

		/** 변수의 타입입니다. */
		TypePtr type;
		/** 변수의 식별자입니다. */
		Identifier identifier;
		/** 변수의 초기화 식입니다. */
		ExpressionPtr expression;
	};

	/**
	 * @brief 함수 선언문의 구조를 담는 추상 구문 트리의 노드입니다.
	 */
	struct FunctionDeclaration : public Statement
	{
		FunctionDeclaration(TypePtr return_type, Identifier identifier,
			const std::vector<VariableDeclaration>& parameter, StatementPtr body);

		std::string tree_gen(std::size_t depth) const override;
		LLVM::Value code_gen() override;

		/** 함수의 반환 값 타입입니다. */
		TypePtr return_type;
		/** 함수의 식별자입니다. */
		Identifier identifier;
		/** 함수의 매개 변수입니다. */
		std::vector<VariableDeclaration> parameter;
		/** 함수의 몸체입니다. */
		StatementPtr body;
	};
}
