#include "CommandLine.hh"

#include <string>

namespace Dlink
{
	/**
	 * @brief 빈 ParsedCommandLine 인스턴스를 만듭니다.
	 * @param type ParsedCommandLine의 타입입니다.
	 */
	ParsedCommandLine::ParsedCommandLine(Type type)
		: type(type), x(0), y(0), z(0)
	{}
	/**
	 * @brief 값이 하나 있는 ParsedCommandLine 인스턴스를 만듭니다.
	 * @param type ParsedCommandLine의 타입입니다.
	 * @param x 첫번째 값입니다.
	 */
	ParsedCommandLine::ParsedCommandLine(Type type, std::uintptr_t x)
		: type(type), x(x), y(0), z(0)
	{}
	/**
	 * @brief 값이 두개 있는 ParsedCommandLine 인스턴스를 만듭니다.
	 * @param type ParsedCommandLine의 타입입니다.
	 * @param x 첫번째 값입니다.
	 * @param y 두번째 값입니다.
	 */
	ParsedCommandLine::ParsedCommandLine(Type type, std::uintptr_t x, std::uintptr_t y)
		: type(type), x(x), y(y), z(0)
	{}
	/**
	 * @brief 값이 두개 있는 ParsedCommandLine 인스턴스를 만듭니다.
	 * @param type ParsedCommandLine의 타입입니다.
	 * @param x 첫번째 값입니다.
	 * @param y 두번째 값입니다.
	 */
	ParsedCommandLine::ParsedCommandLine(Type type, std::uintptr_t x, std::uintptr_t y, std::uintptr_t z)
		: type(type), x(x), y(y), z(z)
	{}

	/**
	 * @brief 두 ParsedCommandLine 인스턴스가 같은지 비교합니다.
	 * @param parsed_command_line 비교할 인스턴스입니다.
	 * @return 같으면 true, 다르면 fasle를 반환합니다.
	 */
	bool ParsedCommandLine::operator==(const ParsedCommandLine& parsed_command_line) const noexcept
	{
		return type == parsed_command_line.type &&
			x == parsed_command_line.x && y == parsed_command_line.y && z == parsed_command_line.z;
	}
	/**
	 * @brief 두 ParsedCommandLine 인스턴스가 다른지 비교합니다.
	 * @param parsed_command_line 비교할 인스턴스입니다.
	 * @return 같으면 false, 다르면 true를 반환합니다.
	 */
	bool ParsedCommandLine::operator!=(const ParsedCommandLine& parsed_command_line) const noexcept
	{
		return type != parsed_command_line.type ||
			x != parsed_command_line.x || y != parsed_command_line.y || z != parsed_command_line.z;
	}

	/**
	 * @brief 명령줄을 파싱합니다.
	 * @param argc 명령줄의 개수입니다.
	 * @param argv 명령줄 배열입니다.
	 * @exception "std::pair<ParsedCommandLine::Error, std::string>" 파싱 중에 발견된 잘못된 명령의 오류 타입과 오류가 난 명령입니다.
	 * @return 파싱된 명령줄 데이터 리스트입니다.
	 */
	std::vector<ParsedCommandLine> ParseCommandLine(int argc, char** argv)
	{
		std::vector<ParsedCommandLine> result;

		for (int i = 1; i < argc; ++i)
		{
			std::string cmdline = argv[i];

			if (cmdline == "/I")
			{
				result.push_back(ParsedCommandLine(ParsedCommandLine::IR));
			}
			else if (cmdline.substr(0, 2) == "/O")
			{
				long long level = std::stoll(cmdline.substr(2));
				result.push_back(ParsedCommandLine(ParsedCommandLine::Optimize, level));
			}

			else if (cmdline[0] == '/')
				throw std::make_pair(ParsedCommandLine::Unknown, cmdline);
			else
			{
				result.push_back(ParsedCommandLine(ParsedCommandLine::Input, reinterpret_cast<std::uintptr_t>(new std::string(cmdline))));
			}
		}

		return result;
	}
	/**
	 * @brief 파싱된 명령줄 데이터 리스트에 오류가 있는지 확인합니다.
	 * @param parsed_command_line 파싱된 명령줄 데이터 리스트입니다.
	 * @return 가장 처음으로 발견된 명령줄 오류의 타입과 발생 인덱스입니다.
	 */
	std::pair<ParsedCommandLine::Error, std::size_t> CheckError_ParsedCommandLine(const std::vector<ParsedCommandLine>& parsed_command_line)
	{
		bool have_I = false;
		bool have_O = false;
		bool have_i = false;

		std::size_t index = 0;
		for (const auto& cmdline : parsed_command_line)
		{
			switch (cmdline.type)
			{
			case ParsedCommandLine::IR:
			{
				if (!have_I)
				{
					have_I = true;
				}
				else
				{
					return std::make_pair(ParsedCommandLine::Multi_IR, index);
				}
				break;
			}

			case ParsedCommandLine::Optimize:
			{
				if (!have_O)
				{
					have_O = true;
					if (cmdline.x >= 5)
					{
						return std::make_pair(ParsedCommandLine::Invalid_Value, index);
					}
				}
				else
				{
					return std::make_pair(ParsedCommandLine::Multi_Optimize, index);
				}
				break;
			}

			case ParsedCommandLine::Input:
			{
				have_i = true;
			}

			default:
				break;
			}
		}

		if (!have_i)
		{
			return std::make_pair(ParsedCommandLine::CouldntFind_Input, -1);
		}
		return std::make_pair(ParsedCommandLine::Done, -1);
	}
}