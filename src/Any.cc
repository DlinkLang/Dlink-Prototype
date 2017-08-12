#include "Any.hh"
#include "ParseStruct/Root.hh"

namespace Dlink
{
	/**
	 * @brief 빈 Any 인스턴스를 만듭니다.
	 */
	Any::Any()
		: data_(nullptr)
	{}
	/**
	 * @brief 기존 인스턴스를 복사해 새 Value 인스턴스를 만듭니다.
	 * @param any 복사할 기존 인스턴스입니다.
	 */
	Any::Any(const Any& any)
	{
		if (any.is_empty())
			data_ = nullptr;
		else
			data_ = any.data_->copy();
	}

	/**
	 * @brief 다른 Any 인스턴스의 값을 현재 인스턴스에 대입합니다.
	 * @param any 대입할 다른 인스턴스입니다.
	 * @return 현재 인스턴스를 반환합니다.
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
	 * @brief 현재 Any 인스턴스가 비어 있는지 확인합니다.
	 * @details 이 함수는 예외를 발생시키지 않습니다.
	 * @return 현재 인스턴스가 비어 있으면 true를, 비어 있지 않으면 false를 반환합니다.
	 */
	bool Any::is_empty() const noexcept
	{
		return data_ == nullptr;
	}
	/**
	 * @brief 현재 Any 인스턴스에 저장된 값의 타입을 가져옵니다.
	 * @return 현재 인스턴스에 저장된 값의 타입을 반환합니다.
	 * @exception std::runtime_error 현재 인스턴스가 비어 있습니다.
	 */
	const std::type_info& Any::type() const
	{
		if (is_empty())
			throw std::runtime_error("현재 Any 인스턴스는 비어 있습니다.");
		return data_->type();
	}
}

namespace Dlink
{
	/**
	 * @brief 두 Any 인스턴스끼리 이항 더하기 연산을 수행합니다.
	 * @details 각각의 Any 인스턴스에 저장된 값끼리 이항 더하기 연산을 수행하게 됩니다.
	 * @param lhs 이항 더하기 연산에서 왼쪽에 올 Any 인스턴스입니다.
	 * @param rhs 이항 더하기 연산에서 오른쪽에 올 Any 인스턴스입니다.
	 * @param out 이항 더하기 연산의 계산 값을 저장할 Any 인스턴스입니다.
	 * @return 이항 더하기 연산을 성공했다면 true, 실패했다면 false를 반환합니다.
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
	 * @brief 두 Any 인스턴스끼리 이항 빼기 연산을 수행합니다.
	 * @details 각각의 Any 인스턴스에 저장된 값끼리 이항 빼기 연산을 수행하게 됩니다.
	 * @param lhs 이항 빼기 연산에서 왼쪽에 올 Any 인스턴스입니다.
	 * @param rhs 이항 빼기 연산에서 오른쪽에 올 Any 인스턴스입니다.
	 * @param out 이항 빼기 연산의 계산 값을 저장할 Any 인스턴스입니다.
	 * @return 이항 빼기 연산을 성공했다면 true, 실패했다면 false를 반환합니다.
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
	 * @brief 두 Any 인스턴스끼리 이항 곱하기 연산을 수행합니다.
	 * @details 각각의 Any 인스턴스에 저장된 값끼리 이항 곱하기 연산을 수행하게 됩니다.
	 * @param lhs 이항 곱하기 연산에서 왼쪽에 올 Any 인스턴스입니다.
	 * @param rhs 이항 곱하기 연산에서 오른쪽에 올 Any 인스턴스입니다.
	 * @param out 이항 곱하기 연산의 계산 값을 저장할 Any 인스턴스입니다.
	 * @return 이항 곱하기 연산을 성공했다면 true, 실패했다면 false를 반환합니다.
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
	 * @brief 두 Any 인스턴스끼리 이항 나누기 연산을 수행합니다.
	 * @details 각각의 Any 인스턴스에 저장된 값끼리 이항 곱하기 연산을 수행하게 됩니다.
	 * @param lhs 이항 나누기 연산에서 왼쪽에 올 Any 인스턴스입니다.
	 * @param rhs 이항 나누기 연산에서 오른쪽에 올 Any 인스턴스입니다.
	 * @param out 이항 나누기 연산의 계산 값을 저장할 Any 인스턴스입니다.
	 * @return 이항 나누기 연산을 성공했다면 true, 실패했다면 false를 반환합니다.
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