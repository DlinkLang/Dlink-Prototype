#pragma once

/**
 * @file Assembler.hh
 * @author kmc7468
 * @breif Assembler 클래스를 정의합니다.
 */

#include "Message/Error.hh"
#include "Message/Warning.hh"
#include "ParseStruct.hh"

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Transforms/Scalar.h"

#include <map>
#include <memory>
#include <thread>

namespace Dlink
{
	/**
	 * @brief 추상 구문 트리를 바탕으로 LLVM IR 코드나 어셈블리어 코드를 만듭니다.
	 * @details 이 클래스는 다른 곳에서 상속받을 수 없습니다.
	 */
	class Assembler final
	{
	public:
		/**
		 * @brief LLVM IR 코드를 만들기 위해 필요한 인스턴스들의 집합입니다.
		 * @details 이 구조체는 다른 곳에서 상속받을 수 없습니다.
		 */
		struct LLVMBuilder final
		{
			LLVMBuilder();

			llvm::LLVMContext context;
			std::shared_ptr<llvm::Module> module;
			llvm::IRBuilder<> builder;
			std::unique_ptr<llvm::legacy::FunctionPassManager> function_pm;
		};

	public:
		Assembler(AST& ast);
		Assembler(const Assembler& assembler) = delete;
		Assembler(Assembler&& assembler) noexcept = delete;
		~Assembler();

	public:
		Assembler& operator=(const Assembler& assembler) = delete;
		Assembler& operator=(Assembler&& assembler) noexcept = delete;
		bool operator==(const Assembler& assembler) const noexcept = delete;
		bool operator!=(const Assembler& assembler) const noexcept = delete;

	public:
		LLVMBuilder& get_llvm_builder() noexcept;
		const LLVMBuilder& get_llvm_builder() const noexcept;
		const Errors& get_errors() const;
		const Warnings& get_warnings() const;

	public:
		static std::map<std::thread::id, Assembler*> assemblers;

	private:
		AST& ast_;
		LLVMBuilder builder_;

		Errors errors_;
		Warnings warnings_;
	};
}