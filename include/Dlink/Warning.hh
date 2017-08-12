#pragma once

/**
 * @file Error.hh
 * @date 2017.08.12
 * @author dev_kr
 * @brief Dlink 컴파일 경고와 관련된 클래스의 집합입니다.
 */

#include "Message.hh"

namespace Dlink
{
	/**
	 * @brief Dlink 코드를 컴파일 하는 중 발생한 경고입니다.
	 * @details 이 클래스는 다른 곳에서 상속받을 수 없습니다.
	 */
	class Warning final : public Message
	{
		using Message::Message;
	};
}
