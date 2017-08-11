#include "CodeGen.hh"
#include "ParseStruct.hh"

#include <utility>

#include "llvm/IR/Instructions.h"

namespace Dlink
{
	namespace LLVM
	{
		llvm::LLVMContext context;
		std::shared_ptr<llvm::Module> module = std::make_shared<llvm::Module>("top", context);
		llvm::IRBuilder<> builder(context);
		std::unique_ptr<llvm::legacy::FunctionPassManager> function_pm;
	}

	/**
	 * @brief 현재 심볼 테이블과 상위 심볼 테이블에서 심볼을 찾습니다.
	 * @param name 찾을 심볼입니다.
	 * @return 심볼을 찾지 못하면 nullptr을 저장하는 LLVM::Value 객체를, 찾으면 해당 심볼의 LLVM Value를 저장하는 LLVM::Value 객체를 반환합니다.
	 */
	LLVM::Value SymbolTable::find(const std::string& name)
	{
		auto find_val = map.find(name);

		if (find_val != map.end())
		{
			return find_val->second;
		}
		else
		{
			return parent == nullptr ? nullptr : parent->find(name);
		}
	}
	/**
	 * @brief 현재 심볼 테이블과 상위 심볼 테이블에서 심볼을 찾습니다.
	 * @param name 찾을 심볼입니다.
	 * @return 심볼을 찾지 못하면 false를, 찾으면 true를 반환합니다.
	 */
	bool SymbolTable::find_bool(const std::string& name) const
	{
		if (map.find(name) != map.end())
		{
			return true;
		}
		else
		{
			return parent == nullptr ? false : parent->find(name);
		}
	}

	/**
	* @brief 현재 심볼 테이블과 상위 심볼 테이블에서 심볼을 찾습니다.
	* @param name 찾을 심볼입니다.
	* @return 심볼을 찾지 못하면 nullptr을, 찾으면 해당 심볼의 LLVM Type을 반환합니다.
	*/
	llvm::Type* TypeSymbolTable::find(const std::string& name)
	{
		auto find_type = map.find(name);

		if (find_type != map.end())
		{
			return find_type->second;
		}
		else
		{
			return parent == nullptr ? nullptr : parent->find(name);
		}
	}
	/**
	* @brief 현재 심볼 테이블과 상위 심볼 테이블에서 심볼을 찾습니다.
	* @param name 찾을 심볼입니다.
	* @return 심볼을 찾지 못하면 false를, 찾으면 true를 반환합니다.
	*/
	bool TypeSymbolTable::find_bool(const std::string& name) const
	{
		if (map.find(name) != map.end())
		{
			return true;
		}
		else
		{
			return parent == nullptr ? false : parent->find(name);
		}
	}

	/** 현재 변수 및 상수 심볼 테이블입니다. */
	SymbolTablePtr symbol_table = std::make_shared<SymbolTable>();
	/** 현재 사용자 정의 타입 심볼 테이블입니다. */
	TypeSymbolTablePtr type_symbol_table = std::make_shared<TypeSymbolTable>();
}