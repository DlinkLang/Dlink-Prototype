#include "ParseStruct.hh"

namespace Dlink
{
	std::string tree_prefix(std::size_t depth)
	{
		return std::string(depth * 6, ' ');
	}
}
