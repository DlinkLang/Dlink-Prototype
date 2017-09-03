#include "LLVMType.hh"

namespace Dlink
{
	namespace LLVM
	{
		/**
		 * @brief �� Type �ν��Ͻ��� ����ϴ�.
		 * @details �� �Լ��� ���ܸ� �߻���Ű�� �ʽ��ϴ�.
		 */
		Type::Type() noexcept
			: type_(nullptr)
		{}
		/**
		 * @brief �� Type �ν��Ͻ��� ����ϴ�.
		 * @details �� �Լ��� ���ܸ� �߻���Ű�� �ʽ��ϴ�.
		 * @param type ������ LLVM Type�Դϴ�.
		 */
		Type::Type(llvm::Type* type) noexcept
			: type_(type)
		{}
		/**
		 * @brief �� Type �ν��Ͻ��� ����ϴ�.
		 * @details �� �Լ��� ���ܸ� �߻���Ű�� �ʽ��ϴ�.
		 * @param type ������ LLVM Type�Դϴ�.
		 * @param is_const const �����ڰ� �پ� �ִ��� �����Դϴ�.
		 */
		Type::Type(llvm::Type* type, bool is_const) noexcept
			: type_(type), is_const_(is_const)
		{}
		/**
		 * @brief �� Type �ν��Ͻ��� ����ϴ�.
		 * @details �� �Լ��� ���ܸ� �߻���Ű�� �ʽ��ϴ�.
		 * @param type ������ LLVM Type�Դϴ�.
		 * @param is_const const �����ڰ� �پ� �ִ��� �����Դϴ�.
		 * @param is_unsigned Ÿ���� unsigned���� �����Դϴ�.
		 */
		Type::Type(llvm::Type* type, bool is_const, bool is_unsigned) noexcept
			: type_(type), is_const_(is_const), is_unsigned_(is_unsigned)
		{}
		/**
		 * @brief ���� �ν��Ͻ��� ������ �� Type �ν��Ͻ��� ����ϴ�.
		 * @details �� �Լ��� ���ܸ� �߻���Ű�� �ʽ��ϴ�.
		 * @param type ������ ���� �ν��Ͻ��Դϴ�.
		 */
		Type::Type(const Type& type) noexcept
			: type_(type.type_), is_const_(type.is_const_), is_unsigned_(type.is_unsigned_)
		{}

		/**
		 * @brief �ٸ� Type �ν��Ͻ��� ���� ���� �ν��Ͻ��� �����մϴ�.
		 * @details �� �Լ��� ���ܸ� �߻���Ű�� �ʽ��ϴ�.
		 * @param type ������ �ٸ� �ν��Ͻ��Դϴ�.
		 * @return ���� �ν��Ͻ��� ��ȯ�մϴ�.
		 */
		Type& Type::operator=(const Type& type) noexcept
		{
			type_ = type.type_;
			is_const_ = type.is_const_;
			is_unsigned_ = type.is_unsigned_;
			return *this;
		}
		/**
		 * @brief ���� �ν��Ͻ��� ����� LLVM Type ���� �ٸ� LLVM Type ���� ������ Ȯ���մϴ�.
		 * @details ���� �ν��Ͻ��� ����� �߰� �����ʹ� ���õ˴ϴ�.
		 * @details �� �Լ��� ���ܸ� �߻���Ű�� �ʽ��ϴ�.
		 * @param type ���� �ٸ� �ν��Ͻ��Դϴ�.
		 * @return �� ���� ������ true, �ٸ��� false�� ��ȯ�մϴ�.
		 */
		bool Type::operator==(llvm::Type* type) const noexcept
		{
			return type_ == type;
		}
		/**
		 * @brief ���� �ν��Ͻ��� ���� �ٸ� �ν��Ͻ��� ���� ������ Ȯ���մϴ�.
		 * @details �� �Լ��� ���ܸ� �߻���Ű�� �ʽ��ϴ�.
		 * @param type ���� �ٸ� �ν��Ͻ��Դϴ�.
		 * @return �� �ν��Ͻ��� ���� ������ true, �ٸ��� false�� ��ȯ�մϴ�.
		 */
		bool Type::operator==(const Type& type) const noexcept
		{
			return type_ == type.type_ && is_const_ == type.is_const_ && is_unsigned_ == type.is_unsigned_;
		}
		/**
		 * @brief ���� �ν��Ͻ��� ����� LLVM Type ���� �ٸ� LLVM Type ���� �ٸ��� Ȯ���մϴ�.
		 * @details ���� �ν��Ͻ��� ����� �߰� �����ʹ� ���õ˴ϴ�.
		 * @details �� �Լ��� ���ܸ� �߻���Ű�� �ʽ��ϴ�.
		 * @param type ���� �ٸ� �ν��Ͻ��Դϴ�.
		 * @return �� ���� �ٸ��� true, ������ false�� ��ȯ�մϴ�.
		 */
		bool Type::operator!=(llvm::Type* type) const noexcept
		{
			return type_ != type;
		}
		/**
		 * @brief ���� �ν��Ͻ��� ���� �ٸ� �ν��Ͻ��� ���� �ٸ��� Ȯ���մϴ�.
		 * @details �� �Լ��� ���ܸ� �߻���Ű�� �ʽ��ϴ�.
		 * @param type ���� �ٸ� �ν��Ͻ��Դϴ�.
		 * @return �� �ν��Ͻ��� ���� �ٸ��� true, ������ false�� ��ȯ�մϴ�.
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
		 * @brief ���� �ν��Ͻ��� ����� LLVM Type ���� �����ɴϴ�.
		 * @details �� �Լ��� ���ܸ� �߻���Ű�� �ʽ��ϴ�.
		 * @return ���� �ν��Ͻ��� ����� LLVM Type ���� ��ȯ�մϴ�.
		 */
		Type::operator llvm::Type*() const noexcept
		{
			return get();
		}

		/**
		 * @brief ���� �ν��Ͻ��� ��� �ִ��� Ȯ���մϴ�.
		 * @details �� �Լ��� ���ܸ� �߻���Ű�� �ʽ��ϴ�.
		 * @return ���� �ν��Ͻ��� ��� ������ true, ��� ���� ������ false�� ��ȯ�մϴ�.
		 */
		bool Type::empty() const noexcept
		{
			return type_ == nullptr && is_const_ == false && is_unsigned_ == false;
		}
		/**
		 * @brief ���� �ν��Ͻ��� ����� LLVM Type ���� �����ɴϴ�.
		 * @details �� �Լ��� ���ܸ� �߻���Ű�� �ʽ��ϴ�.
		 * @return ���� �ν��Ͻ��� ����� LLVM Type ���� ��ȯ�մϴ�.
		 */
		llvm::Type* Type::get() const noexcept
		{
			return type_;
		}
		/**
		 * @brief const �����ڰ� �پ� �ִ��� ���θ� �����ɴϴ�.
		 * @details �� �Լ��� ���ܸ� �߻���Ű�� �ʽ��ϴ�.
		 * @return const �����ڰ� �پ� �ִ��� ���θ� ��ȯ�մϴ�.
		 */
		bool Type::is_const() const noexcept
		{
			return is_const_;
		}
		/**
		 * @brief Ÿ���� unsigned���� ���θ� �����ɴϴ�.
		 * @details �� �Լ��� ���ܸ� �߻���Ű�� �ʽ��ϴ�.
		 * @return Ÿ���� unsigned���� ���θ� ��ȯ�մϴ�.
		 */
		bool Type::is_unsigned() const noexcept
		{
			return is_unsigned_;
		}
	}
}
