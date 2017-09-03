#include "LLVMValue.hh"

#include "ParseStruct/Root.hh"

namespace Dlink
{
	namespace LLVM
	{
		/**
		 * @brief 빈 Value 인스턴스를 만듭니다.
		 * @details 이 함수는 예외를 발생시키지 않습니다.
		 */
		Value::Value() noexcept
			: value_(nullptr), type_(nullptr)
		{}
		/**
		 * @brief 빈 Value 인스턴스를 만듭니다.
		 * @details 이 함수는 예외를 발생시키지 않습니다.
		 */
		Value::Value(std::nullptr_t) noexcept
			: value_(nullptr), type_(nullptr)
		{}
		/**
		 * @brief 새 Value 인스턴스를 만듭니다.
		 * @details 이 함수는 예외를 발생시키지 않습니다.
		 * @param value 저장할 LLVM Value입니다.
		 * @param type 저장할 LLVM Value의 타입입니다.
		 */
		Value::Value(llvm::Value* value, TypePtr type) noexcept
			: value_(value), type_(type)
		{}
		/**
		 * @brief 기존 인스턴스를 복사해 새 Value 인스턴스를 만듭니다.
		 * @details 이 함수는 예외를 발생시키지 않습니다.
		 * @param value 복사할 기존 인스턴스입니다.
		 */
		Value::Value(const Value& value) noexcept
			: value_(value.value_), type_(value.type_)
		{}

		/**
		 * @brief 다른 Value 인스턴스의 값을 현재 인스턴스에 대입합니다.
		 * @details 이 함수는 예외를 발생시키지 않습니다.
		 * @param value 대입할 다른 인스턴스입니다.
		 * @return 현재 인스턴스를 반환합니다.
		 */
		Value& Value::operator=(const Value& value) noexcept
		{
			value_ = value.value_;
			type_ = value.type_;
			return *this;
		}
		/**
		 * @brief 현재 인스턴스에 저장된 LLVM Value 값과 다른 LLVM Value 값이 같은지 확인합니다.
		 * @details 현재 인스턴스에 저장된 추가 데이터는 무시됩니다.
		 * @details 이 함수는 예외를 발생시키지 않습니다.
		 * @param value 비교할 다른 인스턴스입니다.
		 * @return 두 값이 같으면 true, 다르면 false를 반환합니다.
		 */
		bool Value::operator==(llvm::Value* value) const noexcept
		{
			return value_ == value;
		}
		/**
		 * @brief 현재 인스턴스의 값과 다른 인스턴스의 값이 같은지 확인합니다.
		 * @details 이 함수는 예외를 발생시키지 않습니다.
		 * @param value 비교할 다른 인스턴스입니다.
		 * @return 두 인스턴스의 값이 같으면 true, 다르면 false를 반환합니다.
		 */
		bool Value::operator==(const Value& value) const noexcept
		{
			return value_ == value.value_ && type_ == value.type_;
		}
		/**
		 * @brief 현재 인스턴스에 저장된 LLVM Value 값과 다른 LLVM Value 값이 다른지 확인합니다.
		 * @details 현재 인스턴스에 저장된 추가 데이터는 무시됩니다.
		 * @details 이 함수는 예외를 발생시키지 않습니다.
		 * @param value 비교할 다른 인스턴스입니다.
		 * @return 두 값이 다르면 true, 같으면 false를 반환합니다.
		 */
		bool Value::operator!=(llvm::Value* value) const noexcept
		{
			return value_ != value;
		}
		/**
		 * @brief 현재 인스턴스의 값과 다른 인스턴스의 값이 다른지 확인합니다.
		 * @details 이 함수는 예외를 발생시키지 않습니다.
		 * @param value 비교할 다른 인스턴스입니다.
		 * @return 두 인스턴스의 값이 다르면 true, 같으면 false를 반환합니다.
		 */
		bool Value::operator!=(const Value& value) const noexcept
		{
			return value_ != value.value_ || type_ != value.type_;
		}
		/**
		 * @brief 현재 인스턴스에 저장된 LLVM Value 값을 가져옵니다.
		 * @details 이 함수는 예외를 발생시키지 않습니다.
		 * @return 현재 인스턴스에 저장된 LLVM Value 값을 반환합니다.
		 */
		Value::operator llvm::Value*() const noexcept
		{
			return get();
		}

		/**
		 * @brief 현재 인스턴스가 비어 있는지 확인합니다.
		 * @details 이 함수는 예외를 발생시키지 않습니다.
		 * @return 현재 인스턴스가 비어 있으면 true, 비어 있지 않으면 false를 반환합니다.
		 */
		bool Value::empty() const noexcept
		{
			return value_ == nullptr;
		}
		/**
		 * @brief 현재 인스턴스에 저장된 LLVM Value 값을 가져옵니다.
		 * @details 이 함수는 예외를 발생시키지 않습니다.
		 * @return 현재 인스턴스에 저장된 LLVM Value 값을 반환합니다.
		 */
		llvm::Value* Value::get() const noexcept
		{
			return value_;
		}
		/**
		 * @brief 현재 인스턴스에 저장된 LLVM Value 값의 타입을 가져옵니다.
		 * @details 이 함수는 예외를 발생시키지 않습니다.
		 * @return 현재 인스턴스에 저장된 LLVM Value 값의 타입을 반환합니다.
		 */
		TypePtr Value::type() const noexcept
		{
			return type_;
		}
	}
}
