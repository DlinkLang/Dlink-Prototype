#include "ParseStruct/Branch.hh"
#include "CodeGen.hh"

namespace Dlink
{
	extern std::string tree_prefix(std::size_t depth);

	/**
	 * @brief �� IfBranch �ν��Ͻ��� ����ϴ�.
	 * @param token �� ��带 ����µ� ���� ���� ù��° ��ū�Դϴ�.
	 * @param cond ���ǽ��Դϴ�.
	 * @param body ���ǽ��� ���϶� ������ ���Դϴ�.
	 * @param else_body ���ǽ��� �����϶� ������ ���Դϴ�.
	 */
	IfBranch::IfBranch(const Token& token, ExpressionPtr cond, StatementPtr body, std::vector<StatementPtr> else_body)
		: Statement(token), cond(cond), body(body), else_body(else_body)
	{}
	std::string IfBranch::tree_gen(std::size_t depth) const
	{
		std::string result;

		result += tree_prefix(depth) + "IfBranch:\n";
		++depth;
		result += tree_prefix(depth) + "cond:\n" + cond->tree_gen(depth + 1) + '\n';
		result += tree_prefix(depth) + "body:\n" + cond->tree_gen(depth + 1) + '\n';
		result += tree_prefix(depth) + "else_body:\n";

		++depth;
		for (const auto& eb : else_body)
		{
			result += eb->tree_gen(depth) + '\n';
		}
		--depth;

		result.erase(result.end() - 1);
		return result;
	}
	LLVM::Value IfBranch::code_gen()
	{
		llvm::Function* parent = LLVM::builder().GetInsertBlock()->getParent();

		llvm::BasicBlock* if_block;
		llvm::BasicBlock* then_block;
		llvm::BasicBlock* else_block;
		llvm::BasicBlock* end_block;

		if_block = llvm::BasicBlock::Create(LLVM::context(), "if_cmp", parent);
		then_block = llvm::BasicBlock::Create(LLVM::context(), "if_then", parent);

		if (else_body.size() != 0)
		{
			// TODO
		}

		end_block = llvm::BasicBlock::Create(LLVM::context(), "if_end", parent);

		LLVM::builder().CreateBr(if_block);
		LLVM::builder().SetInsertPoint(if_block);

		LLVM::Value cond_value = cond->code_gen();
		llvm::Value* cond_cmp = LLVM::builder().CreateICmpNE(cond_value,
			llvm::ConstantInt::get(cond_value.get()->getType(), 0, true));

		if (else_body.size() != 0)
		{
			// TODO
		}
		else
		{
			LLVM::builder().CreateCondBr(cond_cmp, then_block, end_block);

			LLVM::builder().SetInsertPoint(then_block);
			LLVM::Value ret = body->code_gen();
			LLVM::builder().CreateBr(end_block);

			LLVM::builder().SetInsertPoint(end_block);

			return ret;
		}
	}
}