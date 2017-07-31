#pragma once

/**
 * @file ParseStruct.hh
 * @date 2017.07.28
 * @author dev_kr, kmc7468
 * @brief Dlink 코드 파서의 결과가 생성하는 AST의 노드들을 정의합니다.
 */

#include <cstdint>
#include <memory>
#include <string>
#include <iostream>
#include <vector>

#include "llvm/IR/Value.h"

namespace Dlink
{
    struct Node
    {
        /**
         * @brief 현재 노드의 트리형 구조를 std::string 타입으로 시각화 시킵니다.
         * @param depth 전체 트리에서 현재 노드의 깊이입니다.
         * @return 현재 노드의 트리형 구조를 시각화 시킨 값을 반환합니다.
         */
        virtual std::string tree_gen(std::size_t depth) = 0;
		/**
		 * @brief 현재 노드의 트리형 구조를 LLVM IR 코드로 만듭니다.
		 * @return [추가바람]
		 */
		virtual llvm::Value* code_gen() = 0;
    };

    struct Expression : public Node
    {};

    struct Statement : public Node
    {};
}

namespace Dlink
{
    /**
     * @brief Dlink 연산자의 종류입니다.
     */
    enum class Operator
    {
        None = 0,

        Plus, /**< 이항 덧셈, 단항 양의 부호 연산자입니다. */
        Minus, /**< 이항 뺄셈, 단항 음의 부호 연산자입니다. */
        Multiply, /**< 이항 곱셈 연산자입니다. */
        Divide, /**< 이항 나눗셈 연산자입니다. */
    };

    using ExpressionPtr = std::shared_ptr<Expression>;
    using StatementPtr = std::shared_ptr<Statement>;
   

}

namespace Dlink
{
    /**
     * @brief 한 개 이상의 Statement들의 집합입니다.
     */
    struct Block final : public Statement
    {
        std::vector<StatementPtr> statements;

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
        ExpressionPtr expression;

        ExpressionStatement(ExpressionPtr expression_) : expression(expression_)
        {}
        
        std::string tree_gen(std::size_t depth) override;
		llvm::Value* code_gen() override;
    };
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

        std::string tree_gen(std::size_t depth) override;
		llvm::Value* code_gen() override;
    };
    
    /**
     * @brief 이항 연산의 구조를 담는 파싱 노드입니다.
     */
    struct BinaryOperation final : public Expression
    {
        Operator op;
        ExpressionPtr lhs, rhs;

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
        Operator op;
        ExpressionPtr rhs;

        UnaryOperation(Operator op_, ExpressionPtr rhs_)
            : op(op_), rhs(rhs_)
        {}
        
        std::string tree_gen(std::size_t depth) override;
		llvm::Value* code_gen() override;
	};
}
