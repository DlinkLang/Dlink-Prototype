#include "ParseStruct/Operation.hh"
#include "CodeGen.hh"

#include <iostream>

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
	 * @brief 새 Integer32 인스턴스를 만듭니다.
	 * @details 이 함수는 예외를 발생시키지 않습니다.
	 * @param token 이 노드를 만드는데 사용된 가장 첫번째 토큰입니다.
	 * @param data 32비트 부호 있는 정수 상수입니다.
	 */
	Integer32::Integer32(const Token& token, std::int32_t data) noexcept
		: Expression(token), data(data)
	{}

	std::string Integer32::tree_gen(std::size_t depth) const
	{
		return tree_prefix(depth) + "Integer32(" + std::to_string(data) + ')';
	}
	LLVM::Value Integer32::code_gen()
	{
		return LLVM::builder.getInt32(data);
	}
	bool Integer32::evaluate(Any& out)
	{
		out = static_cast<std::int64_t>(data);
		return true;
	}
}

namespace Dlink
{
	/**
	 * @brief 새 BinaryOperation 인스턴스를 만듭니다.
	 * @param token 이 노드를 만드는데 사용된 가장 첫번째 토큰입니다.
	 * @param op 연산자 타입입니다.
	 * @param lhs 좌측 피연산자입니다.
	 * @param rhs 우측 피연산자입니다.
	 */
	BinaryOperation::BinaryOperation(const Token& token, TokenType op, ExpressionPtr lhs, ExpressionPtr rhs)
		: Expression(token), op(op), lhs(lhs), rhs(rhs)
	{}

	std::string BinaryOperation::tree_gen(std::size_t depth) const
	{
		std::string tree = tree_prefix(depth) + "BinaryOperation:\n";
		++depth;
		tree += tree_prefix(depth) + "lhs:\n";
		tree += lhs->tree_gen(depth + 1) + '\n';
		tree += tree_prefix(depth) + "rhs:\n";
		tree += rhs->tree_gen(depth + 1) + '\n';
		tree += tree_prefix(depth) + "op:\n";
		tree += tree_prefix(depth + 1) + operator_string(op) + '(' + token_map.at(op) + ')';

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
			// TODO: 임시 방안
			return LLVM::builder.CreateSDiv(lhs_value, rhs_value);
		
		case TokenType::assign:
		{
			Identifier* dest;
			if ((dest = dynamic_cast<Identifier*>(lhs.get())))
			{
				return LLVM::builder.CreateStore(rhs_value, symbol_table->find(dest->id));
			}
			
			// TODO: 에러 메세지 채워주세요.
			// TODO: 5 = 3; 이런 상황이랄까요. 식별자가 아닌 곳에 대입을 하는 상황입니다.
			throw Error(token, "TODO");
		}

		default:
			return nullptr;
		}
	}
	bool BinaryOperation::evaluate(Any& out)
	{
		Any lhs_eval, rhs_eval;
		bool lhs_eval_ok = lhs->evaluate(lhs_eval);
		bool rhs_eval_ok = rhs->evaluate(rhs_eval);

		if (lhs_eval_ok && rhs_eval_ok)
		{
			Any eval;
			bool eval_ok;

			switch (op)
			{
			case TokenType::plus:
				eval_ok = any_add(lhs_eval, rhs_eval, eval);
				break;

			case TokenType::minus:
				eval_ok = any_sub(lhs_eval, rhs_eval, eval);
				break;

			case TokenType::multiply:
				eval_ok = any_mul(lhs_eval, rhs_eval, eval);
				break;

			case TokenType::divide:
				eval_ok = any_div(lhs_eval, rhs_eval, eval);
				break;

			default:
				return false;
			}

			if (eval_ok)
			{
				out = eval;
				return true;
			}
		}

		return false;
	}

	/**
	 * @brief 새 UnaryOperation 인스턴스를 만듭니다.
	 * @param token 이 노드를 만드는데 사용된 가장 첫번째 토큰입니다.
	 * @param op 연산자 타입입니다.
	 * @param rhs 피연산자입니다.
	 */
	UnaryOperation::UnaryOperation(const Token& token, TokenType op, ExpressionPtr rhs)
		: Expression(token), op(op), rhs(rhs)
	{}

	std::string UnaryOperation::tree_gen(std::size_t depth) const
	{
		std::string tree = tree_prefix(depth) + "UnaryOperation:\n";
		++depth;
		tree += tree_prefix(depth) + "rhs:\n";
		tree += rhs->tree_gen(depth + 1) + '\n';
		tree += tree_prefix(depth) + "op:\n";
		tree += tree_prefix(depth + 1) + operator_string(op) + '(' + token_map.at(op) + ')';

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
			// TODO: 오류 처리
			return LLVM::builder.getFalse();
		}
	}
	bool UnaryOperation::evaluate(Any& out)
	{
		Any rhs_eval;
		bool rhs_eval_ok = rhs->evaluate(rhs_eval);

		if (rhs_eval_ok)
		{
			Any eval;
			bool eval_ok;

			switch (op)
			{
			case TokenType::plus:
				eval_ok = any_add(0, rhs_eval, eval);
				break;

			case TokenType::minus:
				eval_ok = any_sub(0, rhs_eval, eval);
				break;

			default:
				return false;
			}

			if (eval_ok)
			{
				out = eval;
				return true;
			}
		}

		return false;
	}

	/**
	 * @brief 새 FunctionCallOperation 인스턴스를 만듭니다.
	 * @param token 이 노드를 만드는데 사용된 가장 첫번째 토큰입니다.
	 * @param identifier 호출할 함수의 식별자입니다.
	 * @param argument 인수입니다.
	 */
	FunctionCallOperation::FunctionCallOperation(const Token& token, ExpressionPtr func_expr, const std::vector<ExpressionPtr>& arugment)
		: Expression(token), func_expr(func_expr), argument(arugment)
	{}
	std::string FunctionCallOperation::tree_gen(std::size_t depth) const
	{
		std::string result;
		result += tree_prefix(depth) + "FunctionCallOperation:\n";
		++depth;
		result += tree_prefix(depth) + "func_expr:\n";
		result += func_expr->tree_gen(depth+1) + '\n';
		result += tree_prefix(depth) + "argument:\n";
		++depth;
		for (auto arg : argument)
		{
			result += arg->tree_gen(depth) + '\n';
		}

		return result;
	}
	LLVM::Value FunctionCallOperation::code_gen()
	{
		llvm::Function* function;

		Identifier* dest;
		if ((dest = dynamic_cast<Identifier*>(func_expr.get())))
		{
			function = dynamic_cast<llvm::Function*>(symbol_table->find(dest->id).get());
		}
		else
		{
			function = dynamic_cast<llvm::Function*>(func_expr->code_gen().get());
		}
		

		if (function)
		{
			std::vector<llvm::Value*> arg_real;

			for (auto arg : argument)
			{
				arg_real.push_back(arg->code_gen());
			}

			return LLVM::builder.CreateCall(function, arg_real);
		}
		else
		{
			throw Error(token, "Expected callable function expression");
		}
	}

	/**
	 * @brief 새 UnsafeExpression 인스턴스를 만듭니다.
	 * @param token 이 노드를 만드는데 사용된 가장 첫번째 토큰입니다.
	 * @param expression 안전하지 않은 식입니다.
	 */
	UnsafeExpression::UnsafeExpression(const Token& token, ExpressionPtr expression)
		: Expression(token), expression(expression)
	{}
	std::string UnsafeExpression::tree_gen(std::size_t depth) const
	{
		return tree_prefix(depth) + "UnsafeExpression:\n" +
			tree_prefix(depth + 1) + "expression:\n" +
			expression->tree_gen(depth + 2);
	}
	LLVM::Value UnsafeExpression::code_gen()
	{
		return expression->code_gen();
	}
}

namespace Dlink
{
	/**
	 * @brief 새 ReturnStatement 인스턴스를 만듭니다.
	 * @param token 이 노드를 만드는데 사용된 가장 첫번째 토큰입니다.
	 * @param return_expr 반환할 식입니다.
	 */
	ReturnStatement::ReturnStatement(const Token& token, ExpressionPtr return_expr)
		: Statement(token), return_expr(return_expr)
	{}

	std::string ReturnStatement::tree_gen(std::size_t depth) const
	{
		std::string tree = tree_prefix(depth) + "ReturnStatement:\n";
		if (return_expr)
		{
			tree += return_expr->tree_gen(depth + 1);
		}
		else
		{
			tree += tree_prefix(depth + 1) + "empty";
		}

		return tree;
	}
	LLVM::Value ReturnStatement::code_gen()
	{
		if (return_expr)
		{
			if (LLVM::builder.getCurrentFunctionReturnType() == LLVM::builder.getVoidTy())
			{
				throw Error(token, "Unexpected value return statement in void function");
			}
			return LLVM::builder.CreateRet(return_expr->code_gen());
		}
		else
		{
			if (LLVM::builder.getCurrentFunctionReturnType() != LLVM::builder.getVoidTy())
			{
				throw Error(token, "Expected value return statement in non-void returning function");
			}
			return LLVM::builder.CreateRetVoid();
		}
	}
}
