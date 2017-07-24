#pragma once

#include <istream>
#include <memory>

#include "Scanner.hh"
#include "parser.tab.hh"

namespace Dlink
{
	class Driver
	{
public:
		Driver() = default;

		void parse(std::istream& iss);
		void parse_helper(std::istream& stream);
private:
		std::unique_ptr<Dlink::Parser>  parser  = nullptr;
		std::unique_ptr<Dlink::Scanner> scanner = nullptr;
	};
}

