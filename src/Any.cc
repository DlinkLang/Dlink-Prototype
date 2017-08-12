#include "Any.hh"

namespace Dlink
{
	/**
	 * @brief �� Any �ν��Ͻ��� ����ϴ�.
	 */
	Any::Any()
		: data_(nullptr)
	{}
	/**
	 * @brief ���� �ν��Ͻ��� ������ �� Value �ν��Ͻ��� ����ϴ�.
	 * @param any ������ ���� �ν��Ͻ��Դϴ�.
	 */
	Any::Any(const Any& any)
	{
		if (any.is_empty())
			data_ = nullptr;
		else
			data_ = any.data_->copy();
	}

	/**
	 * @brief �ٸ� Any �ν��Ͻ��� ���� ���� �ν��Ͻ��� �����մϴ�.
	 * @param any ������ �ٸ� �ν��Ͻ��Դϴ�.
	 * @return ���� �ν��Ͻ��� ��ȯ�մϴ�.
	 */
	Any& Any::operator=(const Any& any)
	{
		if (any.is_empty())
			data_ = nullptr;
		else
			data_ = any.data_->copy();
		return *this;
	}

	/**
	 * @brief ���� Any �ν��Ͻ��� ��� �ִ��� Ȯ���մϴ�.
	 * @details �� �Լ��� ���ܸ� �߻���Ű�� �ʽ��ϴ�.
	 * @return ���� �ν��Ͻ��� ��� ������ true��, ��� ���� ������ false�� ��ȯ�մϴ�.
	 */
	bool Any::is_empty() const noexcept
	{
		return data_ == nullptr;
	}
	/**
	 * @brief ���� Any �ν��Ͻ��� ����� ���� Ÿ���� �����ɴϴ�.
	 * @return ���� �ν��Ͻ��� ����� ���� Ÿ���� ��ȯ�մϴ�.
	 * @exception std::runtime_error ���� �ν��Ͻ��� ��� �ֽ��ϴ�.
	 */
	const std::type_info& Any::type() const
	{
		if (is_empty())
			throw std::runtime_error("���� Any �ν��Ͻ��� ��� �ֽ��ϴ�.");
		return data_->type();
	}
}