#include "Error.hh"

namespace Dlink
{
	/**
	 * @brief 기존 인스턴스를 복사해 새 Errors 인스턴스를 만듭니다.
	 * @param errors 복사할 기존 인스턴스입니다.
	 */
	Errors::Errors(const Errors& errors)
		: errors_(errors.errors_)
	{}
	/**
	 * @brief 기존 인스턴스의 필드를 이동해 새 Errors 인스턴스를 만듭니다.
	 * @details 이 함수는 예외를 발생시키지 않습니다.
	 * @param errors 이동할 기존 인스턴스입니다.
	 */
	Errors::Errors(Errors&& errors) noexcept
		: errors_(errors.errors_)
	{}

	/**
	 * @brief 에러를 추가합니다.
	 * @details 이미 추가한 에러라면 추가하지 않습니다.
	 * @param error 추가할 에러입니다.
	 */
	void Errors::add_error(const Error& error)
	{
		const Token& error_token = error.message_token();

		for (auto old_error : errors_)
		{
			const Token& old_token = old_error.message_token();

			if (error.what() == old_error.what() &&
				error_token.line == old_token.line &&
				error_token.col == old_token.col)
			{
				return;
			}
		}

		errors_.push_back(error);
	}
	/**
	 * @brief 추가된 에러 집합을 가져옵니다.
	 * @details 이 함수는 예외를 발생시키지 않습니다.
	 * @return 추가된 에러 집합입니다.
	 */
	const std::vector<Error>& Errors::get_errors() const noexcept
	{
		return errors_;
	}
}
