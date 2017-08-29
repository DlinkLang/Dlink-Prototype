#include "Assembler.hh"

namespace Dlink
{
	Assembler::LLVMBuilder::LLVMBuilder()
		: context(), module(std::make_shared<llvm::Module>("top", context)),
		builder(context), function_pm()
	{}

	Assembler::Assembler(AST& ast)
		: ast_(ast)
	{
		assemblers[std::this_thread::get_id()] = this;
	}
	Assembler::~Assembler()
	{
		std::vector<std::map<std::thread::id, Assembler*>::iterator> iters;

		for (auto iter = assemblers.begin(); iter != assemblers.end(); ++iter)
		{
			if (iter->second == this)
			{
				iters.push_back(iter);
			}
		}

		for (auto iter = iters.cbegin(); iter < iters.cend(); ++iter)
		{
			assemblers.erase(*iter);
		}
	}

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

	std::map<std::thread::id, Assembler*> Assembler::assemblers = {};
}