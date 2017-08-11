#pragma once

/**
 * @file Type.hh
 * @date 2017.08.10
 * @author dev_kr, kmc7468
 * @brief Dlink 코드 파서의 결과가 생성하는 추상 구문 트리 노드들 중 타입과 관련된 노드들을 정의합니다.
 */

#include <string>

#include "llvm/IR/Type.h"

#include "Root.hh"
#include "../LLVMValue.hh"

namespace Dlink
{
	/**
	 * @brief 한정자 등이 없는 순수한 타입입니다.
	 * @details 이 구조체는 다른 곳에서 상속받을 수 없습니다.
	 */
	struct SimpleType final : public Type
	{
		SimpleType(const Token& token, const std::string& identifier);
		SimpleType(const Token& token, const std::string& identifier, bool is_unsigned);

		std::string tree_gen(std::size_t depth) const override;
		llvm::Type* get_type() override;

		/** 타입의 식별자입니다. */
		const std::string identifier;
		/**
		 * @brief 타입이 unsigned인지 여부입니다.
		 * @details 만약 타입이 unsigned라면 true, signed이거나 unsigned/signed 구분이 필요 없는 경우에는 false입니다.
		 */
		const bool is_unsigned;
	};
}
