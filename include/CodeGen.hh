#pragma once

/**
 * @file CodeGen.hh
 * @date 2017.07.29
 * @author kmc7468
 * @brief LLVM IR 코드를 만들기 위해 필요한 값들의 집합입니다.
 */

#include <map>
#include <memory>
#include <string>

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

#include "LLVMValue.hh"

namespace Dlink
{
	namespace LLVM
	{
		extern llvm::LLVMContext context;
		extern std::shared_ptr<llvm::Module> module;
		extern llvm::IRBuilder<> builder;
	}

	struct SymbolTable
	{
		/**
		 * @brief 현재 심볼 테이블의 부모 심볼 테이블입니다.
		 */
		std::shared_ptr<SymbolTable> parent;
		
		/**
		 * @brief 심볼 테이블입니다.
		 */
		std::map<std::string, LLVM::Value> map;
	};
	/**
	 * @brief SymbolTable 구조체에 대한 std::shared_ptr 타입입니다.
	 */
	using SymbolTablePtr = std::shared_ptr<SymbolTable>;

	/**
	 * @brief 현재 심볼 테이블입니다.
	 */
	SymbolTablePtr symbol_table;
}
