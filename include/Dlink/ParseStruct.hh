#pragma once

/**
 * @file ParseStruct.hh
 * @author dev_kr, kmc7468
 * @brief Dlink 코드 파서의 결과가 생성하는 AST의 노드들을 정의합니다.
 */

#include "ParseStruct/Root.hh"
#include "ParseStruct/Operation.hh"
#include "ParseStruct/Type.hh"
#include "ParseStruct/Declaration.hh"

#include <ostream>

namespace Dlink
{
	class Assembler;
	class Parser;

	/**
	 * @brief 추상 구문 트리입니다.
	 * @details 이 클래스는 다른 곳에서 상속받을 수 없습니다.
	 */
	class AST final
	{
		friend class Assembler;
		friend class Parser;

	public:
		AST() = default;
		AST(const AST& ast) = delete;
		AST(AST&& ast) noexcept;
		~AST() = default;

	public:
		AST& operator=(const AST& ast) = delete;
		AST& operator=(AST&& ast) noexcept;
		bool operator==(const AST& ast) const noexcept = delete;
		bool operator!=(const AST& ast) const noexcept = delete;

	public:
		void dump() const;
		void dump(std::ostream& stream) const;
		
	private:
		std::shared_ptr<Node> node_ = nullptr;
	};
}