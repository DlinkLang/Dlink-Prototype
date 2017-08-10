#pragma once

/**
 * @file Error.hh
 * @date 2017.08.08
 * @author dev_kr, kmc7468
 * @brief Dlink 컴파일 오류와 관련된 클래스들의 집합입니다.
 */

#include <string>
#include <vector>

#include "Token.hh"

namespace Dlink
{
	/**
	 * @brief Dlink 코드를 컴파일 하는 중 발생한 에러입니다.
	 * @details 이 클래스는 다른 곳에서 상속받을 수 없습니다.
	 */
	class Error final
	{
	public:
		Error(const Token& error_token, const std::string& error_message);
		Error(const Error& error);
		Error(Error&& error) noexcept;
		~Error() = default;

	public:
		Error& operator=(const Error& error) = delete;
		Error& operator=(Error&& error) noexcept = delete;
		bool operator==(const Error& error) const noexcept = delete;
		bool operator!=(const Error& error) const noexcept = delete;

	public:
		std::string what() const;
		const Token& error_token() const noexcept;

	private:
		Token error_token_;
		std::string error_message_;
	};

	/**
	 * @brief Dlink 코드를 컴파일 하는 중 발생한 에러 집합입니다.
	 * @details 이 클래스는 다른 곳에서 상속받을 수 없습니다.
	 */
	class Errors final
	{
	public:
		/** 빈 Errors 인스턴스를 만듭니다. */
		Errors() = default;
		Errors(const Errors& errors);
		Errors(Errors&& errors) noexcept;
		~Errors() = default;

	public:
		Errors& operator=(const Errors& errors) = delete;
		Errors& operator=(Errors&& errors) noexcept = delete;
		bool operator==(const Errors& errors) const noexcept = delete;
		bool operator!=(const Errors& errors) const noexcept = delete;

	public:
		void add_error(const Error& error);
		const std::vector<Error>& get_errors() const noexcept;

	private:
		std::vector<Error> errors_;
	};
}
