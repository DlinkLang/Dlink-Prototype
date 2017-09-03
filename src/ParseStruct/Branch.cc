#include "ParseStruct/Branch.hh"
#include "CodeGen.hh"

namespace Dlink
{
	extern std::string tree_prefix(std::size_t depth);

	/**
	 * @brief 새 IfBranch 인스턴스를 만듭니다.
	 * @param token 이 노드를 만드는데 사용된 가장 첫번째 토큰입니다.
	 * @param cond 조건식입니다.
	 * @param body 조건식이 참일때 실행할 문입니다.
	 * @param else_body 조건식이 거짓일때 실행할 문입니다.
	 */
	IfBranch::IfBranch(const Token& token, ExpressionPtr cond, StatementPtr body, StatementPtr else_body) 
		: Statement(token), cond(cond), body(body), else_body(else_body)
	{}
	std::string IfBranch::tree_gen(std::size_t depth) const
	{
		std::string result;

		result += tree_prefix(depth) + "IfBranch:\n";
		++depth;
		result += tree_prefix(depth) + "cond:\n" + cond->tree_gen(depth + 1) + '\n';
		result += tree_prefix(depth) + "body:\n" + body->tree_gen(depth + 1) + '\n';
		result += tree_prefix(depth) + "else_body:\n";

		++depth;
		result += else_body->tree_gen(depth) + '\n';
		--depth;

		result.erase(result.end() - 1);
		return result;
	}
	LLVM::Value IfBranch::code_gen()
	{
		llvm::Function* parent_func = LLVM::builder().GetInsertBlock()->getParent();

		llvm::BasicBlock* if_block;
		llvm::BasicBlock* then_block;
		llvm::BasicBlock* else_block;
		llvm::BasicBlock* endif_block;

		if (else_body)
		{
			if_block = llvm::BasicBlock::Create(LLVM::context(), "if", parent_func);
			then_block = llvm::BasicBlock::Create(LLVM::context(), "then", parent_func);
			else_block = llvm::BasicBlock::Create(LLVM::context(), "else", parent_func);
			endif_block = llvm::BasicBlock::Create(LLVM::context(), "endif", parent_func);
		}
		else
		{
			if_block = llvm::BasicBlock::Create(LLVM::context(), "if", parent_func);
			then_block = llvm::BasicBlock::Create(LLVM::context(), "then", parent_func);
			endif_block = llvm::BasicBlock::Create(LLVM::context(), "endif", parent_func);
		}

		LLVM::builder().CreateBr(if_block);
		LLVM::builder().SetInsertPoint(if_block);

		llvm::Value* cond_value = cond->code_gen();
		llvm::Value* cond_cmp = LLVM::builder().CreateICmpNE(cond_value, llvm::ConstantInt::get(cond_value->getType(), 0, true), "branch");

		if (else_body)
		{
			LLVM::builder().CreateCondBr(cond_cmp, then_block, else_block);
			LLVM::builder().SetInsertPoint(then_block);
			body->code_gen();
			LLVM::builder().CreateBr(endif_block);
			LLVM::builder().SetInsertPoint(else_block);
			else_body->code_gen();
			LLVM::builder().CreateBr(endif_block);
			LLVM::builder().SetInsertPoint(endif_block);
		}
		else
		{
			LLVM::builder().CreateCondBr(cond_cmp, then_block, endif_block);
			LLVM::builder().SetInsertPoint(then_block);
			body->code_gen();
			LLVM::builder().CreateBr(endif_block);
			LLVM::builder().SetInsertPoint(endif_block);
		}

		return cond_cmp;
	}
}
