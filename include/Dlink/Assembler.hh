#pragma once

/**
 * @file Assembler.hh
 * @author kmc7468
 * @breif Assembler 클래스를 정의합니다.
 */

#include "ParseStruct.hh"

namespace Dlink
{
	/**
	 * @brief 추상 구문 트리를 바탕으로 LLVM IR이나 어셈블리어 코드를 만듭니다.
	 * @details 이 클래스는 다른 곳에서 상속받을 수 없습니다.
	 */
	class Assembler final
	{
	public:
		Assembler(AST& ast);
		Assembler(const Assembler& assembler) = delete;
		Assembler(Assembler&& assembler) noexcept = delete;
		~Assembler() = default;

	public:
		Assembler& operator=(const Assembler& assembler) = delete;
		Assembler& operator=(Assembler&& assembler) noexcept = delete;
		bool operator==(const Assembler& assembler) const noexcept = delete;
		bool operator!=(const Assembler& assembler) const noexcept = delete;

	public:

	private:
		AST& ast_;
	};
}