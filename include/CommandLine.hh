#pragma once

/**
 * @file CommandLine.hh
 * @date 2017.07.24
 * @author kmc7468
 * @brief 명령줄과 관련된 기능들의 집합입니다.
 */

#include <cstdint>
#include <vector>

namespace Dlink
{
	/**
	 * @brief 파싱된 명령줄 데이터입니다.
	 */
	class ParsedCommandLine final
	{
	public:
		/**
		 * @brief 파싱된 명령줄 데이터 타입입니다.
		 */
		enum ParsedCommandLineType
		{
			None = 0,
			
			IR, /**< LLVM IR로 된 파일로 번역합니다. */
			Optimize, /**< 최적화 수준입니다. */
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