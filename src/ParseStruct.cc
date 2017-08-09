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
	std::string Identifer::tree_gen(std::size_t depth)
	{
		return tree_prefix(depth) + "Identifier(\"" + id + "\")\n";
	}

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

	std::string Scope::tree_gen(std::size_t depth)
	{
		std::string tree = tree_prefix(depth) + "Scope Start\n";

		++depth;
		for (StatementPtr statement : statements)
		{
			tree += statement->tree_gen(depth) + "\n";
		}
		--depth;

		tree += tree_prefix(depth) + "Scope End\n";

		return tree;
	}

	std::string ExpressionStatement::tree_gen(std::size_t depth)
	{
		return expression->tree_gen(depth);
	}
}

namespace Dlink
{
	std::string operator_string(TokenType operator_type)
	{
		switch (operator_type)
		{
		case TokenType::plus:
			return "+";
		case TokenType::increment:
			return "++";
		case TokenType::plus_assign:
			return "+=";

		case TokenType::minus:
			return "-";
		case TokenType::decrement:
			return "--";
		case TokenType::minus_assign:
			return "-=";

		case TokenType::multiply:
			return "*";
		case TokenType::multiply_assign:
			return "*=";

		case TokenType::divide:
			return "/";
		case TokenType::divide_assign:
			return "/=";

		case TokenType::modulo:
			return "%";
		case TokenType::modulo_assign:
			return "%=";

		case TokenType::assign:
			return "=";
		case TokenType::equal:
			return "==";
		case TokenType::noteq:
			return "!=";
		case TokenType::greater:
			return ">";
		case TokenType::eqgreater:
			return ">=";
		case TokenType::less:
			return "<";
		case TokenType::eqless:
			return "<=";

		case TokenType::logic_and:
			return "&&";
		case TokenType::logic_or:
			return "||";

		case TokenType::bit_not:
			return "~";
		case TokenType::bit_and:
			return "&";
		case TokenType::bit_and_assign:
			return "&=";
		case TokenType::bit_or:
			return "|";
		case TokenType::bit_or_assign:
			return "|=";
		case TokenType::bit_xor:
			return "^";
		case TokenType::bit_xor_assign:
			return "^=";
		case TokenType::bit_lshift:
			return "<<";
		case TokenType::bit_lshift_assign:
			return "<<=";
		case TokenType::bit_rshift:
			return ">>";
		case TokenType::bit_rshift_assign:
			return ">>=";

		case TokenType::dot:
			return ".";

		default:
			return "";
		}
	}

	std::string Integer32::tree_gen(std::size_t depth)
	{
		return tree_prefix(depth) + "Integer32(" + std::to_string(data) + ")\n";
	}

	std::string BinaryOperation::tree_gen(std::size_t depth)
	{
		std::string tree = tree_prefix(depth) + "BinaryOperation:\n";
		++depth;
		tree += tree_prefix(depth) + "lhs: \n";
		tree += lhs->tree_gen(depth + 1) + '\n';
		tree += tree_prefix(depth) + "rhs: \n";
		tree += rhs->tree_gen(depth + 1);
		tree += tree_prefix(depth) + "op: \n";
		tree += tree_prefix(depth + 1) + operator_string(op) + '(' + token_map.at(op) + ")\n";

		return tree;
	}

	std::string UnaryOperation::tree_gen(std::size_t depth)
	{
		std::string tree = tree_prefix(depth) + "UnaryOperation:\n";
		++depth;
		tree += tree_prefix(depth) + "rhs:\n";
		tree += rhs->tree_gen(depth + 1);
		tree += tree_prefix(depth) + "op: \n";
		tree += tree_prefix(depth + 1) + operator_string(op) + '(' + token_map.at(op) + ")\n";

		return tree;
	}
}

namespace Dlink
{
	std::string SimpleType::tree_gen(std::size_t depth)
	{
		return tree_prefix(depth) + "SimpleType(" + identifier.id + ")\n";
	}
}

namespace Dlink
{
	std::string VariableDeclaration::tree_gen(std::size_t depth)
	{
		std::string result;
		result += tree_prefix(depth) + "VariableDeclaration:\n";
		++depth;
		result += tree_prefix(depth) + "identifier: \n" + identifier.tree_gen(depth + 1) + "\n";
		result += tree_prefix(depth) + "type: \n" + type->tree_gen(depth + 1) + "\n";
		if (expression)
			result += tree_prefix(depth) + "expression: \n" + expression->tree_gen(depth + 1) + "\n";
		else
			result += tree_prefix(depth) + "expression: empty\n";

		return result;
	}
}