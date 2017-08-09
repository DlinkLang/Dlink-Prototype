#pragma once

/**
 * @file LLVMValue.hh
 * @date 2017.08.09
 * @author kmc7468
 * @brief llvm::Value* ���۸� �����մϴ�.
 */

#include "llvm/IR/Instructions.h"

namespace Dlink
{
	namespace LLVM
	{
		/**
		 * @brief llvm::Value* �����Դϴ�.
		 */
		class Value final
		{
		public:
			Value() noexcept;
			Value(llvm::Value* value) noexcept;
			Value(const Value& value) noexcept;
			~Value() = default;

		public:
			Value& operator=(const Value& value) noexcept;
			bool operator==(llvm::Value* value) const noexcept;
			bool operator==(const Value& value) const noexcept;
			bool operator!=(llvm::Value* value) const noexcept;
			bool operator!=(const Value& value) const noexcept;
			operator llvm::Value*() const noexcept;

		public:
			llvm::Value* get() const noexcept;

		private:
			llvm::Value* value_;
		};
	}
}