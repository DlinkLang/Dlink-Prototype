#include "Driver.hh"

namespace Dlink
{
	/**
	 * @brief 입력 스트림을 받아 입력을 받을 수 있는지 확인하고 스캐너와 파서를 초기화하고 파싱을 한뒤 성공 여부를 반환합니다.
	 * @param stream 입력 스트림입니다.
	 * @return 파싱에 성공하면 0, 실패하면 0 이외의 수를 반환합니다.
	 */
	int Driver::parse(std::istream& stream)
	{
		if(!stream.good() && stream.eof())
		{
			return -1;
		}

		lexer = std::make_unique<Dlink::Lexer>(&stream);
		parser = std::make_unique<Dlink::Parser>((*lexer), (*this));

		return parser->parse();
	}
}
