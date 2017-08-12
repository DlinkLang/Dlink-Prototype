#include "Warning.hh"

namespace Dlink
{
	/**
	 * @brief 기존 인스턴스를 복사해 새 Warnings 인스턴스를 만듭니다.
	 * @param warnings 복사할 기존 인스턴스입니다.
	 */
	Warnings::Warnings(const Warnings& warnings)
		: warnings_(warnings.warnings_)
	{}
	/**
	 * @brief 기존 인스턴스의 필드를 이동해 새 Warnings 인스턴스를 만듭니다.
	 * @details 이 함수는 예외를 발생시키지 않습니다.
	 * @param warnings 이동할 기존 인스턴스입니다.
	 */
	Warnings::Warnings(Warnings&& warnings) noexcept
		: warnings_(warnings.warnings_)
	{}

	/**
	 * @brief 경고를 추가합니다.
	 * @details 이미 추가한 경고라면 추가하지 않습니다.
	 * @param warning 추가할 경고입니다.
	 */
	void Warnings::add_warning(const Warning& warning)
	{
		const Token& warning_token = warning.message_token();

		for (auto old_warning : warnings_)
		{
			const Token& old_token = old_warning.message_token();

			if (warning.what() == old_warning.what() &&
				warning_token.line == old_token.line &&
				warning_token.col == old_token.col)
			{
				return;
			}
		}

		warnings_.push_back(warning);
	}
	/**
	 * @brief 추가된 경고 집합을 가져옵니다.
	 * @details 이 함수는 예외를 발생시키지 않습니다.
	 * @return 추가된 경고 집합입니다.
	 */
	const std::vector<Warning>& Warnings::get_warnings() const noexcept
	{
		return warnings_;
	}
}
