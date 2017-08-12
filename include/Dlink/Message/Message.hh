#pragma once

/**
 * @file Message.hh
 * @date 2017.08.12
 * @author dev_kr, kmc7468
 * @brief Dlink 컴파일 메세지의 베이스 클래스인 Message를 정의합니다.
 */

#include <string>
#include <vector>

#include "../Token.hh"

namespace Dlink
{
	/**
	 * @brief Dlink 코드를 컴파일 하는 중 출력될 메세지들의 베이스 클래스입니다.
	 */
	class Message
	{
	public:
		Message(const Token& message_token, const std::string& message);
		Message(const Message& message);
		Message(Message&& error) noexcept;
		~Message() = default;

	public:
		Message& operator=(const Message& message) = delete;
		Message& operator=(Message&& message) noexcept = delete;
		bool operator==(const Message& message) const noexcept = delete;
		bool operator!=(const Message& message) const noexcept = delete;

	public:
		std::string what() const;
		const Token& message_token() const noexcept;

	private:
		Token message_token_;
		std::string message_;
	};

}
