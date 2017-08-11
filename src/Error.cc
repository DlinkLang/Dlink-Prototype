#include "Error.hh"

#include <utility>

namespace Dlink
{
	/**
	 * @brief 새 Error 인스턴스를 만듭니다.
	 * @param error_token 에러가 발생한 부분의 토큰입니다.
	 * @param error_message 에러 메세지입니다.
	 */
	Error::Error(const Token& error_token, const std::string& error_message)
		: error_token_(error_token), error_message_(error_message)
	{}
	/**
	 * @brief 기존 인스턴스를 복사해 새 Error 인스턴스를 만듭니다.
	 * @param error 복사할 기존 인스턴스입니다.
	 */
	Error::Error(const Error& error)
		: error_token_(error.error_token_), error_message_(error.error_message_)
	{}
	/**
	 * @brief 기존 인스턴스의 필드를 이동해 새 Error 인스턴스를 만듭니다.
	 * @details 이 함수는 예외를 발생시키지 않습니다.
	 * @param error 이동할 기존 인스턴스입니다.
	 */
	Error::Error(Error&& error) noexcept
		: error_token_(std::move(error.error_token_)), error_message_(std::move(error.error_message_))
	{}

	/**
	 * @brief 에러 메세지를 가져옵니다.
	 * @return 에러 메세지를 std::string 타입으로 반환합니다.
	 */
	std::string Error::what() const
	{
		return error_message_;
	}
	/**
	 * @brief 에러가 발생한 부분의 토큰을 가져옵니다.
	 * @details 이 함수는 예외를 발생시키지 않습니다.
	 * @return 에러가 발생한 부분의 토큰을 반환합니다.
	 */
	const Token& Error::error_token() const noexcept
	{
		return error_token_;
	}

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
		const Token& error_token = error.error_token();

		for (auto old_error : errors_)
		{
			const Token& old_token = old_error.error_token();

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
