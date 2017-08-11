#pragma once

/**
 * @file CodeGen.hh
 * @date 2017.08.10
 * @author kmc7468
 * @brief LLVM IR 코드를 만들기 위해 필요한 값들의 집합입니다.
 */

#include <map>
#include <memory>
#include <string>

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LegacyPassManager.h"
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
		extern std::unique_ptr<llvm::legacy::FunctionPassManager> function_pm;
	}

	/**
	 * @brief 변수 및 상수, 함수 심볼 테이블입니다.
	 * @details 사용할 수 있는 변수 및 상수, 함수 심볼을 저장합니다.
	 * @details 이 구조체는 다른 곳에서 상속받을 수 없습니다.
	 */
	struct SymbolTable final
	{
		LLVM::Value find(const std::string& name);

		/** 현재 심볼 테이블의 상위 심볼 테이블입니다. */
		std::shared_ptr<SymbolTable> parent = nullptr;
		/** 변수 및 상수, 함수 심볼 목록입니다. */
		std::map<std::string, LLVM::Value> map;
	};
	/** SymbolTable 구조체에 대한 std::shared_ptr 타입입니다. */
	using SymbolTablePtr = std::shared_ptr<SymbolTable>;

	/**
	 * @brief 타입 심볼 테이블입니다.
	 * @details 사용할 수 있는 사용자 정의 타입 심볼을 저장합니다.
	 * @details 이 구조체는 다른 곳에서 상속받을 수 없습니다.
	 */
	struct TypeSymbolTable final
	{
		llvm::Type* find(const std::string& name);

		/** 현재 심볼 테이블의 상위 심볼 테이블입니다. */
		std::shared_ptr<TypeSymbolTable> parent = nullptr;
		/** 사용자 정의 타입 심볼 목록입니다. */
		std::map<std::string, llvm::Type*> map;
	};
	/** TypeSymbolTable 구조체에 대한 std::shared_ptr 타입입니다. */
	using TypeSymbolTablePtr = std::shared_ptr<TypeSymbolTable>;

	extern SymbolTablePtr symbol_table;
	extern TypeSymbolTablePtr type_symbol_table;
}
