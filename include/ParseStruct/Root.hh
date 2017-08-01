#pragma once

/**
 * @file Root.hh
 * @date 2017.08.01
 * @author dev_kr, kmc7468
 * @brief Dlink 코드 파서의 결과가 생성하는 AST의 노드들 중 기반이 되는 노드들을 정의합니다.
 */

#include <memory>
#include <string>
#include <vector>

#include "llvm/IR/Value.h"

namespace Dlink
{
	/**
	 * @brief 루트 파싱 노드입니다.
	 */
	struct Node
	{
		/**
		 * @brief 현재 노드의 트리형 구조를 std::string 타입으로 시각화합니다.
		 * @param depth 전체 트리에서 현재 노드의 깊이입니다.
		 * @return 현재 노드의 트리형 구조를 시각화 시킨 값을 반환합니다.
		 */
		virtual std::string tree_gen(std::size_t depth) = 0;

		/**
		 * @brief 현재 노드의 트리형 구조를 LLVM IR 코드로 만듭니다.
		 * @return 파싱 노드에서 생성한 llvm::Value*를 반환합니다. 생성한 값이 없을 경우 nullptr을 반환합니다.
		 */
		virtual llvm::Value* code_gen() = 0;
	};

	struct Expression : public Node
	{};

	struct Statement : public Node
	{};

	struct Type
	{
		/**
		 * @brief 현재 타입 노드의 트리형 구조를 std::string 타입으로 시각화합니다.
		 * @param depth 전체 트리에서 현재 노드의 깊이입니다.
		 * @return 현재 노드의 트리형 구조를 시각화 시킨 값을 반환합니다.
		 */
		virtual std::string tree_gen(std::size_t depth) = 0;

		/**
		 * @brief 현재 타입 노드를 LLVM의 Type 객체로 만듭니다.
		 * @return 현재 타입 노드의 트리형 구조를 시각화 시킨 값을 반환합니다.
		 */
		virtual llvm::Type* get_type() = 0;
	};

	using ExpressionPtr = 	std::shared_ptr<Expression>;
	using StatementPtr = 	std::shared_ptr<Statement>;
	using TypePtr =			std::shared_ptr<Type>;
}

namespace Dlink
{
	/**
	 * @brief 한 개 이상의 Statement들의 집합입니다.
	 */
	struct Block final : public Statement
	{
		/**
		 * @brief Statement들을 담아두는 StatementPtr의 std::vector입니다.
		 */
		std::vector<StatementPtr> statements;

		/**
		 * @brief StatementPtr의 std::vector를 받아 멤버 필드 statements를 초기화하고 Block을 생성합니다.
		 */
		Block(std::vector<StatementPtr> statements_) : statements(statements_)
		{}
		
		std::string tree_gen(std::size_t depth) override;
		llvm::Value* code_gen() override;
	};
	
	/**
	 * @brief 한 개의 Expression으로 이루어진 Statement입니다.
	 */
	struct ExpressionStatement final : public Statement
	{
		/**
		 * @brief Expression을 담아두는 ExpressionPtr 타입의 멤버 필드입니다.
		 */
		ExpressionPtr expression;

		/**
		 * @brief ExpressionPtr을 받아 멤버 필드 expression을 초기화하고 ExpressionStatement를 생성합니다.
		 */
		ExpressionStatement(ExpressionPtr expression_) : expression(expression_)
		{}
		
		std::string tree_gen(std::size_t depth) override;
		llvm::Value* code_gen() override;
	};
}