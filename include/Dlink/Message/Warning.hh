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
	
	/**
	 * @brief Dlink 코드를 컴파일 하는 중 발생한 경고 집합입니다.
	 * @details 이 클래스는 다른 곳에서 상속받을 수 없습니다.
	 */
	class Warnings final
	{
	public:
		/** 빈 Warnings 인스턴스를 만듭니다. */
		Warnings() = default;
		Warnings(const Warnings& warnings);
		Warnings(Warnings&& warnings) noexcept;
		~Warnings() = default;

	public:
		Warnings& operator=(const Warnings& warnings) = delete;
		Warnings& operator=(Warnings&& warnings) noexcept = delete;
		bool operator==(const Warnings& warnings) const noexcept = delete;
		bool operator!=(const Warnings& warnings) const noexcept = delete;

	public:
		void add_warning(const Warning& warning);
		const std::vector<Warning>& get_warnings() const noexcept;

	private:
		std::vector<Warning> warnings_;
	};
}
