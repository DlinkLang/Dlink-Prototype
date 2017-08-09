#pragma once

/**
 * @file Type.hh
 * @date 2017.08.01
 * @author dev_kr, kmc7468
 * @brief Dlink 코드 파서가 생성하는 AST 노드들 중 타입과 관련된 노드들을 정의합니다.
 */

#include <string>

#include "llvm/IR/Type.h"

#include "Root.hh"

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
