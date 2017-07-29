#include "CodeGen.hh"

namespace Dlink
{
	namespace LLVM
	{
		llvm::LLVMContext context;
		std::shared_ptr<llvm::Module> module = std::make_shared<llvm::Module>("top", context);
		llvm::IRBuilder<> builder(context);
	}
}
