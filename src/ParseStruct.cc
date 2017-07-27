#include "ParseStruct.hh"

namespace Dlink
{
	std::string Integer32::treeGen(std::size_t depth)
	{
		return std::string(depth, '\t') + "Integer32(" + std::to_string(data) + ")";
	}
	
	std::string BinaryOP::treeGen(std::size_t depth)
	{
		std::string tree = std::string(depth, '\t') + "BinaryOP:\n";
		tree += std::string(depth, '\t') + "lhs:\n";
		tree += lhs->treeGen(depth+1) + '\n';
		tree += std::string(depth, '\t') + "rhs:\n";
		tree += rhs->treeGen(depth+1) + '\n';

		return tree;
	}
}
