#pragma once

/**
 * @file Assembler.hh
 * @author kmc7468
 * @breif Assembler Ŭ������ �����մϴ�.
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
	 * @brief �߻� ���� Ʈ���� �������� LLVM IR �ڵ峪 ������� �ڵ带 ����ϴ�.
	 * @details �� Ŭ������ �ٸ� ������ ��ӹ��� �� �����ϴ�.
	 */
	class Assembler final
	{
	public:
		/**
		 * @brief LLVM IR �ڵ带 ����� ���� �ʿ��� �ν��Ͻ����� �����Դϴ�.
		 * @details �� ����ü�� �ٸ� ������ ��ӹ��� �� �����ϴ�.
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