#pragma once

/**
 * @file Branch.hh
 * @author kmc7468
 * @brief Dlink 코드 파서의 결과가 생성하는 추상 구문 트리의 노드들 중 분기와 관련된 노드들을 정의합니다.
 */

#include "Root.hh"

#include <vector>

namespace Dlink
{
	/**
	 * @brief if 조건문의 구조를 담는 추상 구문 트리의 노드입니다.
	 * @details 이 구조체는 다른 곳에서 상속받을 수 없습니다.
	 */
	struct IfBranch final : public Statement
	{
		IfBranch(const Token& token, ExpressionPtr cond, StatementPtr body, StatementPtr else_body = nullptr);

		std::string tree_gen(std::size_t depth) const;
		LLVM::Value code_gen();

		/** 조건식입니다. */
		ExpressionPtr cond;
		/** 조건식이 참일때 실행할 문입니다. */
		StatementPtr body;
		/** 조건식이 거짓일때 실행할 문입니다. */
		StatementPtr else_body;
	};
}
