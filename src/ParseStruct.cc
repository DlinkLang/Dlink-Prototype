#include "ParseStruct.hh"

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

	std::string AST::tree_gen() const
	{
		return node_->tree_gen(0);
	}
}