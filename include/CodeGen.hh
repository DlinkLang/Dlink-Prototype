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
#include "ParseStruct/Root.hh"

namespace Dlink
{
	namespace LLVM
	{
		extern llvm::LLVMContext context;
		extern std::shared_ptr<llvm::Module> module;
		extern llvm::IRBuilder<> builder;
	}

	/**
	 * @brief 심볼 테이블입니다.
	 */
	struct SymbolTable final
	{
		/**
		 * @brief 현재 심볼 테이블의 부모 심볼 테이블입니다.
		 */
		std::shared_ptr<SymbolTable> parent = nullptr;
		
		/**
		 * @brief 실질적인 값을 저장하는 심볼 테이블입니다.
		 */
		std::map<std::string, LLVM::Value> map;

		LLVM::Value find(const std::string& name);
	};
	/**
	 * @brief SymbolTable 구조체에 대한 std::shared_ptr 타입입니다.
	 */
	using SymbolTablePtr = std::shared_ptr<SymbolTable>;

	/**
	 * @brief 타입 심볼 테이블입니다.
	 */
	struct TypeSymbolTable final
	{
		/**
		 * @brief 현재 타입 심볼 테이블의 부모 타입 심볼 테이블입니다.
		 */
		std::shared_ptr<TypeSymbolTable> parent = nullptr;

		/**
		 * @brief 실질적인 값을 저장하는 타입 심볼 테이블입니다.
		 */
		std::map<std::string, llvm::Type*> map;

		llvm::Type* find(const std::string& name);
	};
	/**
	 * @brief TypeSymbolTable 구조체에 대한 std::shared_ptr 타입입니다.
	 */
	using TypeSymbolTablePtr = std::shared_ptr<TypeSymbolTable>;

	/**
	 * @brief 현재 심볼 테이블입니다.
	 */
	extern SymbolTablePtr symbol_table;
	/**
	 * @brief 현재 타입 심볼 테이블입니다.
	 */
	extern TypeSymbolTablePtr type_symbol_table;
}
