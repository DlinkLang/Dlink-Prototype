#include "Any.hh"
#include "ParseStruct/Root.hh"

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

namespace Dlink
{
	/**
	 * @brief �� Any �ν��Ͻ����� ���� ���ϱ� ������ �����մϴ�.
	 * @details ������ Any �ν��Ͻ��� ����� ������ ���� ���ϱ� ������ �����ϰ� �˴ϴ�.
	 * @param lhs ���� ���ϱ� ���꿡�� ���ʿ� �� Any �ν��Ͻ��Դϴ�.
	 * @param rhs ���� ���ϱ� ���꿡�� �����ʿ� �� Any �ν��Ͻ��Դϴ�.
	 * @param out ���� ���ϱ� ������ ��� ���� ������ Any �ν��Ͻ��Դϴ�.
	 * @return ���� ���ϱ� ������ �����ߴٸ� true, �����ߴٸ� false�� ��ȯ�մϴ�.
	 */
	bool Expression::any_add(const Any& lhs, const Any& rhs, Any& out)
	{
		if (lhs.type() == typeid(std::int64_t))
		{
			if (rhs.type() == typeid(std::int64_t))
			{
				out = lhs.get<std::int64_t>() + rhs.get<std::int64_t>();
				return true;
			}
		}

		return false;
	}
	/**
	 * @brief �� Any �ν��Ͻ����� ���� ���� ������ �����մϴ�.
	 * @details ������ Any �ν��Ͻ��� ����� ������ ���� ���� ������ �����ϰ� �˴ϴ�.
	 * @param lhs ���� ���� ���꿡�� ���ʿ� �� Any �ν��Ͻ��Դϴ�.
	 * @param rhs ���� ���� ���꿡�� �����ʿ� �� Any �ν��Ͻ��Դϴ�.
	 * @param out ���� ���� ������ ��� ���� ������ Any �ν��Ͻ��Դϴ�.
	 * @return ���� ���� ������ �����ߴٸ� true, �����ߴٸ� false�� ��ȯ�մϴ�.
	 */
	bool Expression::any_sub(const Any& lhs, const Any& rhs, Any& out)
	{
		if (lhs.type() == typeid(std::int64_t))
		{
			if (rhs.type() == typeid(std::int64_t))
			{
				out = lhs.get<std::int64_t>() - rhs.get<std::int64_t>();
				return true;
			}
		}

		return false;
	}
	/**
	 * @brief �� Any �ν��Ͻ����� ���� ���ϱ� ������ �����մϴ�.
	 * @details ������ Any �ν��Ͻ��� ����� ������ ���� ���ϱ� ������ �����ϰ� �˴ϴ�.
	 * @param lhs ���� ���ϱ� ���꿡�� ���ʿ� �� Any �ν��Ͻ��Դϴ�.
	 * @param rhs ���� ���ϱ� ���꿡�� �����ʿ� �� Any �ν��Ͻ��Դϴ�.
	 * @param out ���� ���ϱ� ������ ��� ���� ������ Any �ν��Ͻ��Դϴ�.
	 * @return ���� ���ϱ� ������ �����ߴٸ� true, �����ߴٸ� false�� ��ȯ�մϴ�.
	 */
	bool Expression::any_mul(const Any& lhs, const Any& rhs, Any& out)
	{
		if (lhs.type() == typeid(std::int64_t))
		{
			if (rhs.type() == typeid(std::int64_t))
			{
				out = lhs.get<std::int64_t>() * rhs.get<std::int64_t>();
				return true;
			}
		}

		return false;
	}
	/**
	 * @brief �� Any �ν��Ͻ����� ���� ������ ������ �����մϴ�.
	 * @details ������ Any �ν��Ͻ��� ����� ������ ���� ���ϱ� ������ �����ϰ� �˴ϴ�.
	 * @param lhs ���� ������ ���꿡�� ���ʿ� �� Any �ν��Ͻ��Դϴ�.
	 * @param rhs ���� ������ ���꿡�� �����ʿ� �� Any �ν��Ͻ��Դϴ�.
	 * @param out ���� ������ ������ ��� ���� ������ Any �ν��Ͻ��Դϴ�.
	 * @return ���� ������ ������ �����ߴٸ� true, �����ߴٸ� false�� ��ȯ�մϴ�.
	 */
	bool Expression::any_div(const Any& lhs, const Any& rhs, Any& out)
	{
		if (lhs.type() == typeid(std::int64_t))
		{
			if (rhs.type() == typeid(std::int64_t))
			{
				out = lhs.get<std::int64_t>() / rhs.get<std::int64_t>();
				return true;
			}
		}

		return false;
	}
}