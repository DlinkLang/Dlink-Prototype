#include "ParseStruct/Operation.hh"
#include "CodeGen.hh"

namespace Dlink
{
	extern std::string tree_prefix(std::size_t depth);
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

	/**
	 * @brief �� Integer32 �ν��Ͻ��� ����ϴ�.
	 * @details �� �Լ��� ���ܸ� �߻���Ű�� �ʽ��ϴ�.
	 * @param data 32��Ʈ ��ȣ �ִ� ���� ����Դϴ�.
	 */
	Integer32::Integer32(std::int32_t data) noexcept
		: data(data)
	{}

	std::string Integer32::tree_gen(std::size_t depth)
	{
		return tree_prefix(depth) + "Integer32(" + std::to_string(data) + ")\n";
	}
	LLVM::Value Integer32::code_gen()
	{
		return LLVM::builder.getInt32(data);
	}
}

namespace Dlink
{
	/**
	 * @brief �� BinaryOperation �ν��Ͻ��� ����ϴ�.
	 * @param op ������ Ÿ���Դϴ�.
	 * @param lhs ���� �ǿ������Դϴ�.
	 * @param rhs ���� �ǿ������Դϴ�.
	 */
	BinaryOperation::BinaryOperation(TokenType op, ExpressionPtr lhs, ExpressionPtr rhs)
		: op(op), lhs(lhs), rhs(rhs)
	{}

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
	LLVM::Value BinaryOperation::code_gen()
	{
		LLVM::Value lhs_value = lhs->code_gen();
		LLVM::Value rhs_value = rhs->code_gen();

		switch (op)
		{
		case TokenType::plus:
			return LLVM::builder.CreateAdd(lhs_value, rhs_value);

		case TokenType::minus:
			return LLVM::builder.CreateSub(lhs_value, rhs_value);

		case TokenType::multiply:
			return LLVM::builder.CreateMul(lhs_value, rhs_value);

		case TokenType::divide:
			// TODO: �ӽ� ���
			return LLVM::builder.CreateSDiv(lhs_value, rhs_value);

		default:
			// TODO: ���� ó��
			return LLVM::builder.getFalse();
		}
	}

	/**
	 * @brief �� UnaryOperation �ν��Ͻ��� ����ϴ�.
	 * @param op ������ Ÿ���Դϴ�.
	 * @param rhs �ǿ������Դϴ�.
	 */
	UnaryOperation::UnaryOperation(TokenType op, ExpressionPtr rhs)
		: op(op), rhs(rhs)
	{}

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
	LLVM::Value UnaryOperation::code_gen()
	{
		LLVM::Value rhs_value = rhs->code_gen();

		switch (op)
		{
		case TokenType::plus:
			return LLVM::builder.CreateMul(LLVM::builder.getInt32(1), rhs_value);

		case TokenType::minus:
			return LLVM::builder.CreateMul(LLVM::builder.getInt32(-1), rhs_value);

		default:
			// TODO: ���� ó��
			return LLVM::builder.getFalse();
		}
	}
}