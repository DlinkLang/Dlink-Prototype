#pragma once

/**
 * @file Root.hh
 * @date 2017.08.01
 * @author dev_kr, kmc7468
 * @brief Dlink 코드 파서의 결과가 생성하는 추상 구문 트리의 노드들 중 기반이 되는 노드들을 정의합니다.
 */

#include <memory>
#include <string>
#include <vector>

#include "llvm/IR/Value.h"

#include "Any.hh"
#include "LLVMValue.hh"
#include "Token.hh"

namespace Dlink
{
	/**
	 * @brief 추상 구문 트리의 루트 노드입니다.
	 */
	struct Node
	{
		Node(const Token& token);

		/**
		 * @brief 이 노드의 트리를 std::string 타입으로 시각화합니다.
		 * @param depth 전체 트리에서 현재 노드의 깊이입니다.
		 * @return 이 노드의 트리형 구조를 시각화 시킨 값을 반환합니다.
		 */
		virtual std::string tree_gen(std::size_t depth) const = 0;
		/**
		 * @brief 이 노드의 트리를 LLVM IR 코드로 만듭니다.
		 * @return 파싱 노드에서 생성한 LLVM::Value를 반환합니다. 생성한 값이 없을 경우 nullptr을 반환합니다.
		 */
		virtual LLVM::Value code_gen() = 0;

		/** 이 노드를 만드는데 사용된 가장 첫번째 토큰입니다. */
		const Token token;
	};

	/**
	 * @brief Dlink 코드에서의 식입니다.
	 */
	struct Expression : public Node
	{
		using Node::Node;

		virtual bool evaluate(Any& out);

	protected:
		bool any_add(const Any& lhs, const Any& rhs, Any& out);
		bool any_sub(const Any& lhs, const Any& rhs, Any& out);
		bool any_mul(const Any& lhs, const Any& rhs, Any& out);
		bool any_div(const Any& lhs, const Any& rhs, Any& out);
	};

	/**
	 * @brief Dlink 코드에서의 문입니다.
	 */
	struct Statement : public Node
	{
		using Node::Node;
	};

	/**
	 * @brief Dlink의 타입(자료형)입니다.
	 */
	struct Type
	{
		Type(const Token& token);

		/**
		 * @brief 현재 타입 노드의 트리를 std::string 타입으로 시각화합니다.
		 * @param depth 전체 트리에서 현재 노드의 깊이입니다.
		 * @return 현재 노드의 트리를 시각화 시킨 값을 반환합니다.
		 */
		virtual std::string tree_gen(std::size_t depth) const = 0;

		/**
		 * @brief 현재 타입 노드를 LLVM Type으로 만듭니다.
		 * @return 만들어진 LLVM Type을 반환합니다.
		 */
		virtual llvm::Type* get_type() = 0;
		
		/** 이 노드를 만드는데 사용된 가장 첫번째 토큰입니다. */
		const Token token;
	};

	/** Expression 구조체에 대한 std::shared_ptr 타입입니다. */
	using ExpressionPtr = std::shared_ptr<Expression>;
	/** Statement 구조체에 대한 std::shared_ptr 타입입니다. */
	using StatementPtr = std::shared_ptr<Statement>;
	/** Type 구조처에 대한 std::shared_ptr 타입입니다. */
	using TypePtr = std::shared_ptr<Type>;
}

namespace Dlink
{
	/**
	 * @brief 식별자입니다.
	 * @details 이 구조체는 다른 곳에서 상속받을 수 없습니다.
	 */
	struct Identifier final : public Expression
	{
		Identifier(const Token& token, const std::string& id);

		std::string tree_gen(std::size_t depth) const override;
		LLVM::Value code_gen() override;

		/** 실질적인 식별자 값입니다. */
		const std::string id;
	};

	/**
	 * @brief 한 개 이상의 Statement들의 집합입니다.
	 */
	struct Block : public Statement
	{
		Block(const Token& token, const std::vector<StatementPtr>& statements);

		std::string tree_gen(std::size_t depth) const override;
		LLVM::Value code_gen() override;

		/** Statemenet들의 집합입니다. */
		std::vector<StatementPtr> statements;
	};

	/**
	 * @brief Dlink 코드에서 중괄호에 의해 감싸지며 객체의 수명을 결정하는 Statement들의 집합입니다.
	 * @details 이 구조체는 다른 곳에서 상속받을 수 없습니다.
	 */
	struct Scope final : public Block
	{
		Scope(const Token& token, const std::vector<StatementPtr>& statements, StatementPtr parent);

		std::string tree_gen(std::size_t depth) const override;
		LLVM::Value code_gen() override;

		/** 현재 Scope의 상위 Block 또는 상위 Scope입니다. */
		StatementPtr parent;
		/** 현재 Scope의 하위 Scope입니다. */
		std::vector<std::shared_ptr<Scope>> child;
	};
	/** Scope 구조체에 대한 std::shared_ptr 타입입니다. */
	using ScopePtr = std::shared_ptr<Scope>;

	/**
	 * @brief 한 개의 식으로 이루어진 Statement입니다.
	 * @details 이 구조체는 다른 곳에서 상속받을 수 없습니다.
	 */
	struct ExpressionStatement final : public Statement
	{
		ExpressionStatement(const Token& token, ExpressionPtr expression);

		std::string tree_gen(std::size_t depth) const override;
		LLVM::Value code_gen() override;

		/** 식입니다. */
		ExpressionPtr expression;
	};
}
