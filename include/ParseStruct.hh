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
		 * @return 파싱 노드에서 생성한 llvm::Value*를 반환합니다. 생성할 값이 없을 경우 nullptr을 반환합니다.
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
         * @brief 현재 타입 노드의 트리형 구조를 std::string 타입으로 시각화 시킵니다.
         * @param depth 전체 트리에서 현재 노드의 깊이입니다.
         * @return 현재 타입 노드의 트리형 구조를 시각화 시킨 값을 반환합니다.
         */
        virtual std::string tree_gen(std::size_t depth) = 0;

		/**
		 * @brief 현재 타입 노드를 LLVM의 Type 객체로 만듭니다.
		 * @return 타입 노드에서 생성한 llvm::Type*을 반환합니다.
		 */
        virtual llvm::Type* get_type() = 0;
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

        Plus, /**< 이항 덧셈, 단항 양의 부호 연산자입니다. */
        Minus, /**< 이항 뺄셈, 단항 음의 부호 연산자입니다. */
        Multiply, /**< 이항 곱셈 연산자입니다. */
        Divide, /**< 이항 나눗셈 연산자입니다. */
    };

    using ExpressionPtr = std::shared_ptr<Expression>;
    using StatementPtr = std::shared_ptr<Statement>;
    using TypePtr = std::shared_ptr<Type>;
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
        /**
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

namespace Dlink
{
    /**
     * @brief int, char와 같이 한정자나 포인터 선언 등이 없는 순수 타입입니다.
     */
    struct SimpleType final : public Type
    {
        /**
         * @brief 타입의 문자열 식별자를 담는 std::string 타입의 멤버 필드입니다.
         */
        const std::string& identifier;

        /**
         * @brief std::string을 받아 멤버 필드 identifier를 초기화하고 SimpleType을 생성합니다.
         */
        SimpleType(const std::string& identifier_)
            : identifier(identifier_)
        {}
        
        std::string tree_gen(std::size_t depth) override;
		llvm::Type* get_type() override;
    };
}
