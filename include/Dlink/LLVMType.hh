#pragma once

/**
* @file LLVMType.hh
* @author kmc7468
* @brief llvm::Type*의 래퍼를 정의합니다.
*/

#include "llvm/IR/Instructions.h"

namespace Dlink
{
	namespace LLVM
	{
		/**
		* @brief llvm::Type* 래퍼입니다.
		* @details 이 클래스는 다른 곳에서 상속받을 수 없습니다.
		*/
		class Type final
		{
		public:
			Type() noexcept;
			Type(llvm::Type* type) noexcept;
			Type(const Type& type) noexcept;
			~Type() = default;

		public:
			Type& operator=(const Type& type) noexcept;
			bool operator==(llvm::Type* type) const noexcept;
			bool operator==(const Type& type) const noexcept;
			bool operator!=(llvm::Type* type) const noexcept;
			bool operator!=(const Type& type) const noexcept;
			const llvm::Type* operator->() const noexcept;
			llvm::Type* operator->() noexcept;
			operator llvm::Type*() const noexcept;

		public:
			bool empty() const noexcept;
			llvm::Type* get() const noexcept;

		private:
			llvm::Type* type_;
		};
	}
}