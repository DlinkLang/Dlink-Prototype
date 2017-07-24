#pragma once

/**
 * @file CommandLine.hh
 * @date 2017.07.24
 * @author kmc7468
 * @brief ����ٰ� ���õ� ��ɵ��� �����Դϴ�.
 */

#include <cstdint>
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
		enum ParsedCommandLineType
		{
			None = 0,
			
			IR, /**< LLVM IR�� �� ���Ϸ� �����մϴ�. */
			Optimize, /**< ����ȭ �����Դϴ�. */
		};

	public:
		ParsedCommandLine(ParsedCommandLineType type);
		ParsedCommandLine(ParsedCommandLineType type, std::uintptr_t x);
		ParsedCommandLine(ParsedCommandLineType type, std::uintptr_t x, std::uintptr_t y);
		ParsedCommandLine(ParsedCommandLineType type, std::uintptr_t x, std::uintptr_t y, std::uintptr_t z);
		ParsedCommandLine(const ParsedCommandLine& parsed_command_line) = default;
		ParsedCommandLine(ParsedCommandLine&& parsed_command_line) noexcept = default;
		~ParsedCommandLine() = default;

	public:
		ParsedCommandLine& operator=(const ParsedCommandLine& parsed_command_line) = default;
		ParsedCommandLine& operator=(ParsedCommandLine&& parsed_command_line) noexcept = default;
		bool operator==(const ParsedCommandLine& parsed_command_line) const noexcept;
		bool operator!=(const ParsedCommandLine& parsed_command_line) const noexcept;

	public:
		ParsedCommandLineType type;
		std::uintptr_t x;
		std::uintptr_t y;
		std::uintptr_t z;
	};

	std::vector<ParsedCommandLine> ParseCommandLine(int argc, char** argv);
}