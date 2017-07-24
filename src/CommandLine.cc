#include "CommandLine.hh"

namespace Dlink
{
	/**
	 * @brief 빈 ParsedCommandLine 인스턴스를 만듭니다.
	 * @param type ParsedCommandLine의 타입입니다.
	 */
	ParsedCommandLine::ParsedCommandLine(ParsedCommandLineType type)
		: type(type), x(0), y(0), z(0)
	{}
	/**
	 * @brief 값이 하나 있는 ParsedCommandLine 인스턴스를 만듭니다.
	 * @param type ParsedCommandLine의 타입입니다.
	 * @param x 첫번째 값입니다.
	 */
	ParsedCommandLine::ParsedCommandLine(ParsedCommandLineType type, std::uintptr_t x)
		: type(type), x(x), y(0), z(0)
	{}
	/**
	 * @brief 값이 두개 있는 ParsedCommandLine 인스턴스를 만듭니다.
	 * @param type ParsedCommandLine의 타입입니다.
	 * @param x 첫번째 값입니다.
	 * @param y 두번째 값입니다.
	 */
	ParsedCommandLine::ParsedCommandLine(ParsedCommandLineType type, std::uintptr_t x, std::uintptr_t y)
		: type(type), x(x), y(y), z(0)
	{}
	/**
	 * @brief 값이 두개 있는 ParsedCommandLine 인스턴스를 만듭니다.
	 * @param type ParsedCommandLine의 타입입니다.
	 * @param x 첫번째 값입니다.
	 * @param y 두번째 값입니다.
	 */
	ParsedCommandLine::ParsedCommandLine(ParsedCommandLineType type, std::uintptr_t x, std::uintptr_t y, std::uintptr_t z)
		: type(type), x(x), y(y), z(z)
	{}

	bool ParsedCommandLine::operator==(const ParsedCommandLine& parsed_command_line) const noexcept
	{
		return type == parsed_command_line.type &&
			x == parsed_command_line.x && y == parsed_command_line.y && z == parsed_command_line.z;
	}
	bool ParsedCommandLine::operator!=(const ParsedCommandLine& parsed_command_line) const noexcept
	{
		return type != parsed_command_line.type ||
			x != parsed_command_line.x || y != parsed_command_line.y || z != parsed_command_line.z;
	}

	std::vector<ParsedCommandLine> ParseCommandLine(int argc, char** argv)
	{
		std::vector<ParsedCommandLine> result;

		for (int i = 1; i < argc; ++i)
		{
			// Do something
		}

		return result;
	}
}