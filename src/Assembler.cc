#include "Assembler.hh"
#include "Init.hh"

//#include "llvm/Transforms/Scalar/GVN.h"

namespace Dlink
{
	Assembler::LLVMBuilder::LLVMBuilder()
		: context(), module(std::make_shared<llvm::Module>("top", context)),
		builder(context), function_pm(nullptr)
	{
		function_pm = std::make_unique<llvm::legacy::FunctionPassManager>(module.get());

		if (opt_level > 0)
		{
			function_pm->add(llvm::createInstructionCombiningPass());
			function_pm->add(llvm::createReassociatePass());
			function_pm->add(llvm::createGVNPass());
			function_pm->add(llvm::createCFGSimplificationPass());
		}

		function_pm->doInitialization();
	}

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

	bool Assembler::to_llvm_ir()
	{
		try
		{
			ast_.node_->preprocess();
			ast_.node_->code_gen();

			return true;
		}
		catch (Error& error)
		{
			errors_.add_error(error);
			return false;
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
	const Errors& Assembler::get_errors() const noexcept
	{
		return errors_;
	}
	Warnings& Assembler::get_warnings() noexcept
	{
		return warnings_;
	}
	const Warnings& Assembler::get_warnings() const noexcept
	{
		return warnings_;
	}

	std::map<std::thread::id, Assembler*> Assembler::assemblers = {};
}
