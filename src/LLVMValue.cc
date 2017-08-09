#include "LLVMValue.hh"

namespace Dlink
{
	namespace LLVM
	{
		Value::Value() noexcept
			: value_(nullptr)
		{}
		Value::Value(llvm::Value* value) noexcept
			: value_(value)
		{}
		Value::Value(const Value& value) noexcept
			: value_(value.value_)
		{}

		Value& Value::operator=(const Value& value) noexcept
		{
			value_ = value.value_;
			return *this;
		}
		bool Value::operator==(llvm::Value* value) const noexcept
		{
			return value_ == value;
		}
		bool Value::operator==(const Value& value) const noexcept
		{
			return value_ == value.value_;
		}
		bool Value::operator!=(llvm::Value* value) const noexcept
		{
			return value_ != value;
		}
		bool Value::operator!=(const Value& value) const noexcept
		{
			return value_ != value.value_;
		}
		Value::operator llvm::Value*() const noexcept
		{
			return get();
		}

		llvm::Value* Value::get() const noexcept
		{
			return value_;
		}
	}
}