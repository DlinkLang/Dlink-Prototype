#pragma once

/**
 * @file CommandLine.hh
 * @date 2017.07.24
 * @author kmc7468
 * @brief ����ٰ� ���õ� ��ɵ��� �����Դϴ�.
 */

#include <cstdint>
#include <utility>
#include <vector>

namespace Dlink
{
	/**
	 * @brief �Ľ̵� ����� �������Դϴ�.
	 */
	class ParsedCommandLine final
	{
	public:
		/**
		 * @brief �Ľ̵� ����� ������ Ÿ���Դϴ�.
		 */
		enum Type
		{
			None = 0,
			
			IR, /**< LLVM IR�� �� ���Ϸ� �����մϴ�. */
			Optimize, /**< ����ȭ �����Դϴ�. */
			Input, /**< �������� �ҽ� �����Դϴ�. */
		};
		/**
		 * @brief �Ľ��ϴ� ���� �߻��� ���� Ÿ���Դϴ�.
		 */
		enum Error
		{
			Done, /**< ����ٿ� ������ �����ϴ�. */
			Unknown, /**< ����ٿ� �� �� ���� ����� �ֽ��ϴ�. */

			Multi_IR, /**< ����ٿ� /IR�� ������ �ֽ��ϴ�. */
			Multi_Optimize, /**< ����ٿ� /O�� ������ �ֽ��ϴ�. */

			No_Input, /**< �������� �ҽ� ������ �����ϴ�. */

			CouldntFind_Input, /**< �������� �ҽ� ������ ã�� ���߽��ϴ�. */
			Invalid_Value, /**< ����� ���� ���� ������ �ֽ��ϴ�. */
		};

	public:
		ParsedCommandLine(Type type);
		ParsedCommandLine(Type type, std::uintptr_t x);
		ParsedCommandLine(Type type, std::uintptr_t x, std::uintptr_t y);
		ParsedCommandLine(Type type, std::uintptr_t x, std::uintptr_t y, std::uintptr_t z);
		/**
		 * @brief ParsedCommandLine �ν��Ͻ��� ������ �� �ν��Ͻ��� ����ϴ�.
		 * @param parsed_command_line ������ �ν��Ͻ��Դϴ�.
		 */
		ParsedCommandLine(const ParsedCommandLine& parsed_command_line) = default;
		~ParsedCommandLine() = default;

	public:
		/**
		 * @brief ParsedCommandLine �ν��Ͻ��� ���� �ν��Ͻ��� �����մϴ�.
		 * @param parsed_command_line ������ �ν��Ͻ��Դϴ�.
		 * @return ���� �ν��Ͻ��Դϴ�.
		 */
		ParsedCommandLine& operator=(const ParsedCommandLine& parsed_command_line) = default;
		bool operator==(const ParsedCommandLine& parsed_command_line) const noexcept;
		bool operator!=(const ParsedCommandLine& parsed_command_line) const noexcept;

	public:
		Type type;
		std::uintptr_t x;
		std::uintptr_t y;
		std::uintptr_t z;
	};

	std::vector<ParsedCommandLine> ParseCommandLine(int argc, char** argv);
	std::pair<ParsedCommandLine::Error, std::size_t> CheckError_ParsedCommandLine(const std::vector<ParsedCommandLine>& parsed_command_line);
}