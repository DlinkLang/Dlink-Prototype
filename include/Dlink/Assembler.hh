#pragma once

/**
 * @file Assembler.hh
 * @author kmc7468
 * @breif Assembler Ŭ������ �����մϴ�.
 */

#include "ParseStruct.hh"

namespace Dlink
{
	/**
	 * @brief �߻� ���� Ʈ���� �������� LLVM IR�̳� ������� �ڵ带 ����ϴ�.
	 * @details �� Ŭ������ �ٸ� ������ ��ӹ��� �� �����ϴ�.
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