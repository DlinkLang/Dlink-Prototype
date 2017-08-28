#include "ParseStruct.hh"

#include <iostream>
#include <utility>

namespace Dlink
{
	std::string tree_prefix(std::size_t depth)
	{
		return std::string(depth * 4, ' ');
	}

	AST::AST(AST&& ast) noexcept
		: node_(std::move(ast.node_))
	{}

	AST& AST::operator=(AST&& ast) noexcept
	{
		node_ = std::move(ast.node_);
		return *this;
	}

	void AST::dump() const
	{
		dump(std::cout);
	}
	void AST::dump(std::ostream& stream) const
	{
		std::cout << node_->tree_gen(0) << '\n';
	}
}