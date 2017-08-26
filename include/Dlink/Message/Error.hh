#pragma once

/**
 * @file Error.hh
 * @author dev_kr
 * @brief Dlink 컴파일 에러와 관련된 클래스의 집합입니다.
 */

#include "Message.hh"

namespace Dlink
{
	/**
	 * @brief Dlink 코드를 컴파일 하는 중 발생한 에러입니다.
	 * @details 이 클래스는 다른 곳에서 상속받을 수 없습니다.
	 */
	class Error final : public Message
	{
		using Message::Message;
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
