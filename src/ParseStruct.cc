#include "ParseStruct.hh"

namespace Dlink
{
	std::string tree_prefix(std::size_t depth)
	{
		return std::string(depth * 6, ' ');
	}
}

namespace Dlink
{
	std::string Block::tree_gen(std::size_t depth)
	{
		std::string tree = tree_prefix(depth) + "Block Start\n";

		for (StatementPtr statement : statements)
		{
			tree += statement->tree_gen(depth + 1) + "\n";
		}

		tree += tree_prefix(depth) + "Block End\n";

		return tree;
	}

	std::string ExpressionStatement::tree_gen(std::size_t depth)
	{
		return expression->tree_gen(depth);
	}
}

namespace Dlink
{

	std::string Integer32::tree_gen(std::size_t depth)
	{
		return tree_prefix(depth) + "Integer32(" + std::to_string(data) + ")";
	}

	std::string BinaryOperation::tree_gen(std::size_t depth)
	{
		std::string tree = tree_prefix(depth) + "BinaryOperation:\n";
		++depth;
		tree += tree_prefix(depth) + "lhs: \n";
		tree += lhs->tree_gen(depth + 1) + '\n';
		tree += tree_prefix(depth) + "rhs: \n";
		tree += rhs->tree_gen(depth + 1);

		return tree;
	}

	std::string UnaryOperation::tree_gen(std::size_t depth)
	{
		std::string tree = tree_prefix(depth) + "UnaryOperation:\n";
		++depth;
		tree += tree_prefix(depth) + "rhs:\n";
		tree += rhs->tree_gen(depth + 1);

		return tree;
	}
}

namespace Dlink
{
	std::string SimpleType::tree_gen(std::size_t depth)
	{
		return tree_prefix(depth) + "SimpleType(" + identifier + ")";
	}
}

namespace Dlink
{
	std::string VariableDeclaration::tree_gen(std::size_t depth)
	{
		std::string result;
		result += tree_prefix(depth) + "VariableDeclaration:\n";
		++depth;
		result += tree_prefix(depth) + "identifier:\n" + identifier + "\n";
		result += tree_prefix(depth) + "type:\n" + type->tree_gen(depth + 1) + "\n";
		result += tree_prefix(depth) + "expression:\n" + expression->tree_gen(depth + 1) + "\n";
		return result;
	}
}
