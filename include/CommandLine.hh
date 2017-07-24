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
		/**
		 * @brief ParsedCommandLine 인스턴스를 복사해 새 인스턴스를 만듭니다.
		 * @param parsed_command_line 복사할 인스턴스입니다.
		 */
		ParsedCommandLine(const ParsedCommandLine& parsed_command_line) = default;
		~ParsedCommandLine() = default;

	public:
		/**
		 * @brief ParsedCommandLine 인스턴스를 현재 인스턴스에 대입합니다.
		 * @param parsed_command_line 대입할 인스턴스입니다.
		 * @return 현재 인스턴스입니다.
		 */
		ParsedCommandLine& operator=(const ParsedCommandLine& parsed_command_line) = default;
		bool operator==(const ParsedCommandLine& parsed_command_line) const noexcept;
		bool operator!=(const ParsedCommandLine& parsed_command_line) const noexcept;

	public:
		ParsedCommandLineType type;
		std::uintptr_t x;
		std::uintptr_t y;
		std::uintptr_t z;
	};

	std::vector<ParsedCommandLine> ParseCommandLine(int argc, char** argv);
	bool CheckError_ParsedCommandLine(const std::vector<ParsedCommandLine>& parsed_command_line);
}