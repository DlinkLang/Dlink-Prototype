#include "Message.hh"

#include <utility>

namespace Dlink
{
	/**
	 * @brief 새 Message 인스턴스를 만듭니다.
	 * @param message_token 에러가 발생한 부분의 토큰입니다.
	 * @param message_message 에러 메세지입니다.
	 */
	Message::Message(const Token& message_token, const std::string& message)
		: message_token_(message_token), message_(message)
	{}
	/**
	 * @brief 기존 인스턴스를 복사해 새 Message 인스턴스를 만듭니다.
	 * @param message 복사할 기존 인스턴스입니다.
	 */
	Message::Message(const Message& message)
		: message_token_(message.message_token_), message_(message.message_)
	{}
	/**
	 * @brief 기존 인스턴스의 필드를 이동해 새 Message 인스턴스를 만듭니다.
	 * @details 이 함수는 예외를 발생시키지 않습니다.
	 * @param message 이동할 기존 인스턴스입니다.
	 */
	Message::Message(Message&& message) noexcept
		: message_token_(std::move(message.message_token_)), message_(std::move(message.message_))
	{}

	/**
	 * @brief 에러 메세지를 가져옵니다.
	 * @return 에러 메세지를 std::string 타입으로 반환합니다.
	 */
	std::string Message::what() const
	{
		return message_;
	}
	/**
	 * @brief 에러가 발생한 부분의 토큰을 가져옵니다.
	 * @details 이 함수는 예외를 발생시키지 않습니다.
	 * @return 에러가 발생한 부분의 토큰을 반환합니다.
	 */
	const Token& Message::message_token() const noexcept
	{
		return message_token_;
	}
}
