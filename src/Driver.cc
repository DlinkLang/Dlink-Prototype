#include "Driver.hh"

namespace Dlink
{
	void Driver::parse(std::istream& stream )
	{
		if(!stream.good() && stream.eof())
		{
			return;
		}

		parse_helper(stream); 
	}

	void Driver::parse_helper( std::istream &stream )
	{

		scanner = std::make_unique<Dlink::Scanner>(&stream);
		parser = std::make_unique<Dlink::Parser>((*scanner), (*this));

		const int accept = 0;
		if( parser->parse() != accept )
		{
			std::cerr << "Parse failed!!\n";
		}
	}
}
