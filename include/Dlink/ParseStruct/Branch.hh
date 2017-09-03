#pragma once

/**
 * @file Branch.hh
 * @author kmc7468
 * @brief Dlink �ڵ� �ļ��� ����� �����ϴ� �߻� ���� Ʈ���� ���� �� �б�� ���õ� ������ �����մϴ�.
 */

#include "Root.hh"

#include <vector>

namespace Dlink
{
	/**
	 * @brief if ���ǹ��� ������ ��� �߻� ���� Ʈ���� ����Դϴ�.
	 * @details �� ����ü�� �ٸ� ������ ��ӹ��� �� �����ϴ�.
	 */
	struct IfBranch final : public Statement
	{
		IfBranch(const Token& token, ExpressionPtr cond, StatementPtr body, StatementPtr else_body = nullptr);

		std::string tree_gen(std::size_t depth) const;
		LLVM::Value code_gen();

		/** ���ǽ��Դϴ�. */
		ExpressionPtr cond;
		/** ���ǽ��� ���϶� ������ ���Դϴ�. */
		StatementPtr body;
		/** ���ǽ��� �����϶� ������ ���Դϴ�. */
		StatementPtr else_body;
	};
}
