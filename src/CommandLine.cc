#include "CommandLine.hh"

#include <string>

namespace Dlink
{
	/**
	 * @brief �� ParsedCommandLine �ν��Ͻ��� ����ϴ�.
	 * @param type ParsedCommandLine�� Ÿ���Դϴ�.
	 */
	ParsedCommandLine::ParsedCommandLine(Type type)
		: type(type), x(0), y(0), z(0)
	{}
	/**
	 * @brief ���� �ϳ� �ִ� ParsedCommandLine �ν��Ͻ��� ����ϴ�.
	 * @param type ParsedCommandLine�� Ÿ���Դϴ�.
	 * @param x ù��° ���Դϴ�.
	 */
	ParsedCommandLine::ParsedCommandLine(Type type, std::uintptr_t x)
		: type(type), x(x), y(0), z(0)
	{}
	/**
	 * @brief ���� �ΰ� �ִ� ParsedCommandLine �ν��Ͻ��� ����ϴ�.
	 * @param type ParsedCommandLine�� Ÿ���Դϴ�.
	 * @param x ù��° ���Դϴ�.
	 * @param y �ι�° ���Դϴ�.
	 */
	ParsedCommandLine::ParsedCommandLine(Type type, std::uintptr_t x, std::uintptr_t y)
		: type(type), x(x), y(y), z(0)
	{}
	/**
	 * @brief ���� �ΰ� �ִ� ParsedCommandLine �ν��Ͻ��� ����ϴ�.
	 * @param type ParsedCommandLine�� Ÿ���Դϴ�.
	 * @param x ù��° ���Դϴ�.
	 * @param y �ι�° ���Դϴ�.
	 */
	ParsedCommandLine::ParsedCommandLine(Type type, std::uintptr_t x, std::uintptr_t y, std::uintptr_t z)
		: type(type), x(x), y(y), z(z)
	{}

	/**
	 * @brief �� ParsedCommandLine �ν��Ͻ��� ������ ���մϴ�.
	 * @param parsed_command_line ���� �ν��Ͻ��Դϴ�.
	 * @return ������ true, �ٸ��� fasle�� ��ȯ�մϴ�.
	 */
	bool ParsedCommandLine::operator==(const ParsedCommandLine& parsed_command_line) const noexcept
	{
		return type == parsed_command_line.type &&
			x == parsed_command_line.x && y == parsed_command_line.y && z == parsed_command_line.z;
	}
	/**
	 * @brief �� ParsedCommandLine �ν��Ͻ��� �ٸ��� ���մϴ�.
	 * @param parsed_command_line ���� �ν��Ͻ��Դϴ�.
	 * @return ������ false, �ٸ��� true�� ��ȯ�մϴ�.
	 */
	bool ParsedCommandLine::operator!=(const ParsedCommandLine& parsed_command_line) const noexcept
	{
		return type != parsed_command_line.type ||
			x != parsed_command_line.x || y != parsed_command_line.y || z != parsed_command_line.z;
	}

	/**
	 * @brief ������� �Ľ��մϴ�.
	 * @param argc ������� �����Դϴ�.
	 * @param argv ����� �迭�Դϴ�.
	 * @exception "std::pair<ParsedCommandLine::Error, std::string>" �Ľ� �߿� �߰ߵ� �߸��� ����� ���� Ÿ�԰� ������ �� ����Դϴ�.
	 * @return �Ľ̵� ����� ������ ����Ʈ�Դϴ�.
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
	 * @brief �Ľ̵� ����� ������ ����Ʈ�� ������ �ִ��� Ȯ���մϴ�.
	 * @param parsed_command_line �Ľ̵� ����� ������ ����Ʈ�Դϴ�.
	 * @return ���� ó������ �߰ߵ� ����� ������ Ÿ�԰� �߻� �ε����Դϴ�.
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