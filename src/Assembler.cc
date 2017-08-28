#include "Assembler.hh"

namespace Dlink
{
	Assembler::LLVMBuilder::LLVMBuilder()
		: context(), module(std::make_shared<llvm::Module>("top", context)),
		builder(context), function_pm()
	{}

	Assembler::Assembler(AST& ast)
		: ast_(ast)
	{}

	Assembler::LLVMBuilder& Assembler::get_llvm_builder() noexcept
	{
		return builder_;
	}
	const Assembler::LLVMBuilder& Assembler::get_llvm_builder() const noexcept
	{
		return builder_;
	}
	const Errors& Assembler::get_errors() const
	{
		return errors_;
	}
	const Warnings& Assembler::get_warnings() const
	{
		return warnings_;
	}
}