#include "LLVMType.hh"

namespace Dlink
{
	namespace LLVM
	{
		/**
		 * @brief 빈 Type 인스턴스를 만듭니다.
		 * @details 이 함수는 예외를 발생시키지 않습니다.
		 */
		Type::Type() noexcept
			: type_(nullptr)
		{}
		/**
		 * @brief 새 Type 인스턴스를 만듭니다.
		 * @details 이 함수는 예외를 발생시키지 않습니다.
		 * @param type 저장할 LLVM Type입니다.
		 */
		Type::Type(llvm::Type* type) noexcept
			: type_(type)
		{}
		/**
		 * @brief 새 Type 인스턴스를 만듭니다.
		 * @details 이 함수는 예외를 발생시키지 않습니다.
		 * @param type 저장할 LLVM Type입니다.
		 * @param is_const const 지정자가 붙어 있는지 여부입니다.
		 */
		Type::Type(llvm::Type* type, bool is_const) noexcept
			: type_(type), is_const_(is_const)
		{}
		/**
		 * @brief 새 Type 인스턴스를 만듭니다.
		 * @details 이 함수는 예외를 발생시키지 않습니다.
		 * @param type 저장할 LLVM Type입니다.
		 * @param is_const const 지정자가 붙어 있는지 여부입니다.
		 * @param is_unsigned 타입이 unsigned인지 여부입니다.
		 */
		Type::Type(llvm::Type* type, bool is_const, bool is_unsigned) noexcept
			: type_(type), is_const_(is_const), is_unsigned_(is_unsigned)
		{}
		/**
		 * @brief 기존 인스턴스를 복사해 새 Type 인스턴스를 만듭니다.
		 * @details 이 함수는 예외를 발생시키지 않습니다.
		 * @param type 복사할 기존 인스턴스입니다.
		 */
		Type::Type(const Type& type) noexcept
			: type_(type.type_), is_const_(type.is_const_), is_unsigned_(type.is_unsigned_)
		{}

		/**
		 * @brief 다른 Type 인스턴스의 값을 현재 인스턴스에 대입합니다.
		 * @details 이 함수는 예외를 발생시키지 않습니다.
		 * @param type 대입할 다른 인스턴스입니다.
		 * @return 현재 인스턴스를 반환합니다.
		 */
		Type& Type::operator=(const Type& type) noexcept
		{
			type_ = type.type_;
			is_const_ = type.is_const_;
			is_unsigned_ = type.is_unsigned_;
			return *this;
		}
		/**
		 * @brief 현재 인스턴스에 저장된 LLVM Type 값과 다른 LLVM Type 값이 같은지 확인합니다.
		 * @details 현재 인스턴스에 저장된 추가 데이터는 무시됩니다.
		 * @details 이 함수는 예외를 발생시키지 않습니다.
		 * @param type 비교할 다른 인스턴스입니다.
		 * @return 두 값이 같으면 true, 다르면 false를 반환합니다.
		 */
		bool Type::operator==(llvm::Type* type) const noexcept
		{
			return type_ == type;
		}
		/**
		 * @brief 현재 인스턴스의 값과 다른 인스턴스의 값이 같은지 확인합니다.
		 * @details 이 함수는 예외를 발생시키지 않습니다.
		 * @param type 비교할 다른 인스턴스입니다.
		 * @return 두 인스턴스의 값이 같으면 true, 다르면 false를 반환합니다.
		 */
		bool Type::operator==(const Type& type) const noexcept
		{
			return type_ == type.type_ && is_const_ == type.is_const_ && is_unsigned_ == type.is_unsigned_;
		}
		/**
		 * @brief 현재 인스턴스에 저장된 LLVM Type 값과 다른 LLVM Type 값이 다른지 확인합니다.
		 * @details 현재 인스턴스에 저장된 추가 데이터는 무시됩니다.
		 * @details 이 함수는 예외를 발생시키지 않습니다.
		 * @param type 비교할 다른 인스턴스입니다.
		 * @return 두 값이 다르면 true, 같으면 false를 반환합니다.
		 */
		bool Type::operator!=(llvm::Type* type) const noexcept
		{
			return type_ != type;
		}
		/**
		 * @brief 현재 인스턴스의 값과 다른 인스턴스의 값이 다른지 확인합니다.
		 * @details 이 함수는 예외를 발생시키지 않습니다.
		 * @param type 비교할 다른 인스턴스입니다.
		 * @return 두 인스턴스의 값이 다르면 true, 같으면 false를 반환합니다.
		 */
		bool Type::operator!=(const Type& type) const noexcept
		{
			return type_ != type.type_ || is_const_ != type.is_const_ || is_unsigned_ != type.is_unsigned_;
		}
		const llvm::Type* Type::operator->() const noexcept
		{
			return type_;
		}
		llvm::Type* Type::operator->() noexcept
		{
			return type_;
		}
		/**
		 * @brief 현재 인스턴스에 저장된 LLVM Type 값을 가져옵니다.
		 * @details 이 함수는 예외를 발생시키지 않습니다.
		 * @return 현재 인스턴스에 저장된 LLVM Type 값을 반환합니다.
		 */
		Type::operator llvm::Type*() const noexcept
		{
			return get();
		}

		/**
		 * @brief 현재 인스턴스가 비어 있는지 확인합니다.
		 * @details 이 함수는 예외를 발생시키지 않습니다.
		 * @return 현재 인스턴스가 비어 있으면 true, 비어 있지 않으면 false를 반환합니다.
		 */
		bool Type::empty() const noexcept
		{
			return type_ == nullptr && is_const_ == false && is_unsigned_ == false;
		}
		/**
		 * @brief 현재 인스턴스에 저장된 LLVM Type 값을 가져옵니다.
		 * @details 이 함수는 예외를 발생시키지 않습니다.
		 * @return 현재 인스턴스에 저장된 LLVM Type 값을 반환합니다.
		 */
		llvm::Type* Type::get() const noexcept
		{
			return type_;
		}
		/**
		 * @brief const 지정자가 붙어 있는지 여부를 가져옵니다.
		 * @details 이 함수는 예외를 발생시키지 않습니다.
		 * @return const 지정자가 붙어 있는지 여부를 반환합니다.
		 */
		bool Type::is_const() const noexcept
		{
			return is_const_;
		}
		/**
		 * @brief 타입이 unsigned인지 여부를 가져옵니다.
		 * @details 이 함수는 예외를 발생시키지 않습니다.
		 * @return 타입이 unsigned인지 여부를 반환합니다.
		 */
		bool Type::is_unsigned() const noexcept
		{
			return is_unsigned_;
		}
	}
}
