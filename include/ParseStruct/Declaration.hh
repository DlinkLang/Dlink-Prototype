#pragma once

/**
 * @file Declaration.hh
 * @date 2017.08.01
 * @author kmc7468
 * @brief Dlink 코드 파서의 결과가 생성하는 AST의 노드들 중 선언과 관련된 노드들을 정의합니다.
 */

#include <memory>
#include <string>

#include "llvm/IR/Value.h"

#include "Root.hh"
#include "../LLVMValue.hh"

namespace Dlink
{
	/**
	 * @brief 변수 선언문의 구조를 담는 파싱 노드입니다.
	 */
	struct VariableDeclaration : public Statement
	{
		/**
		 * @brief 변수의 타입을 담는 TypePtr 타입의 멤버 필드입니다.
		 */
		TypePtr type;

		/**
		 * @brief 변수의 문자열 식별자를 담는 Identifier 타입의 멤버 필드입니다.
		 */
		Identifer identifier;

		/**
		 * @brief 변수의 초기화 식을 담는 ExpressionPtr 타입의 멤버 필드입니다.
		 */
		ExpressionPtr expression;

		VariableDeclaration(TypePtr type_, const std::string& identifier_)
			: type(type_), identifier(identifier_)
		{}
		VariableDeclaration(TypePtr type_, const std::string& identifier_, ExpressionPtr expression_)
			: type(type_), identifier(identifier_), expression(expression_)
		{}

		std::string tree_gen(std::size_t depth) override;
		LLVM::Value code_gen() override;
	};

	/**
	 * @brief 함수 선언문의 구조를 담는 파싱 노드입니다.
	 */
	struct FunctionDeclaration : public Statement
	{
		/**
		 * @brief 함수의 반환 타입을 담는 TypePtr 타입의 멤버 필드입니다.
		 */
		TypePtr return_type;

		/**
		 * @brief 함수의 문자열 식별자를 담는 Identifier 타입의 멤버 필드입니다.
		 */
		Identifer identifier;

		/**
		 * @brief 함수의 매개 변수 목록을 담는 std::vector<VariableDeclaration> 타입의 멤버 필드입니다.
		 */
		std::vector<VariableDeclaration> parameter;

		/**
		 * @brief 함수의 몸체를 담는 StatementPtr 타입의 멤버 필드입니다.
		 */
		StatementPtr body;
	};
}
