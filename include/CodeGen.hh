#pragma once

/**
  * @file CodeGen.hh
  * @date 2017.07.29
  * @author kmc7468
  * @brief LLVM IR 코드를 만들기 위해 필요한 값들의 집합입니다.
  */

#include <memory>

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

namespace Dlink
{
	namespace LLVM
	{
		extern llvm::LLVMContext context;
		extern std::shared_ptr<llvm::Module> module;
		extern llvm::IRBuilder<> builder;
	}
}
