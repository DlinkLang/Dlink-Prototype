#pragma once

/**
 * @file LLVMValue.hh
 * @author kmc7468
 * @brief llvm::Value*의 래퍼를 정의합니다.
 */

#include <memory>

#include "llvm/IR/Instructions.h"

namespace Dlink
{
	struct Type;

	namespace LLVM
	{
		/**
		 * @brief llvm::Value* 래퍼입니다.
		 * @details 이 클래스는 다른 곳에서 상속받을 수 없습니다.
		 */
		class Value final
		{
		public:
			Value() noexcept;
			Value(std::nullptr_t) noexcept;
			Value(llvm::Value* value, std::shared_ptr<Dlink::Type> type) noexcept;
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
			bool empty() const noexcept;
			llvm::Value* get() const noexcept;
			std::shared_ptr<Dlink::Type> type() const noexcept;

		private:
			llvm::Value* value_;
			std::shared_ptr<Dlink::Type> type_;
		};
	}
}
