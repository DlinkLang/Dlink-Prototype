#include <iostream>

#include "ParseStruct/Root.hh"
#include "CodeGen.hh"

namespace Dlink
{
	extern std::string tree_prefix(std::size_t depth);
	
	/**
	 * @brief 이 Node 인스턴스의 멤버를 초기화합니다.
	 * @param token 이 노드를 만드는데 사용된 가장 첫번째 토큰입니다.
	 */
	Node::Node(const Token& token)
		: token(token)
	{}

	/**
	 * @brief 새 Identifier 인스턴스를 만듭니다.
	 * @param token 이 노드를 만드는데 사용된 가장 첫번째 토큰입니다.
	 * @param id 식별자 값입니다.
	 */
	Identifier::Identifier(const Token& token, const std::string& id)
		: Expression(token), id(id)
	{}

	std::string Identifier::tree_gen(std::size_t depth) const
	{
		return tree_prefix(depth) + "Identifier(\"" + id + "\")";
	}
	LLVM::Value Identifier::code_gen()
	{
		LLVM::Value result = symbol_table->find(id);

		if (result == nullptr)
		{
			throw Error(token, "Unbound symbol \"" + id + "\"");
		}

		return result;
	}

	/**
	 * @brief 새 Block 인스턴스를 만듭니다.
	 * @param token 이 노드를 만드는데 사용된 가장 첫번째 토큰입니다.
	 * @param statements Statement들의 집합입니다.
	 */
	Block::Block(const Token& token, const std::vector<StatementPtr>& statements)
		: Statement(token), statements(statements)
	{}

	std::string Block::tree_gen(std::size_t depth) const
	{
		std::string tree = tree_prefix(depth) + "Block Start\n";

		for (StatementPtr statement : statements)
		{
			tree += statement->tree_gen(depth + 1) + '\n';
		}

		tree += tree_prefix(depth) + "Block End";

		return tree;
	}
	LLVM::Value Block::code_gen()
	{
		LLVM::Value last_value;

		for (StatementPtr statement : statements)
		{
			last_value = statement->code_gen();
		}

		return last_value;
	}

	/**
	 * @brief 새 Scope 인스턴스를 만듭니다.
	 * @param token 이 노드를 만드는데 사용된 가장 첫번째 토큰입니다.
	 * @param statements Statement들의 집합입니다.
	 * @param parent 현재 스코프의 상위 스코프입니다.
	 */
	Scope::Scope(const Token& token, const std::vector<StatementPtr>& statements, StatementPtr parent)
		: Block(token, statements), parent(parent)
	{}
	
	std::string Scope::tree_gen(std::size_t depth) const
	{
		std::string tree = tree_prefix(depth) + "Scope Start\n";

		++depth;
		for (StatementPtr statement : statements)
		{
			tree += statement->tree_gen(depth) + '\n';
		}
		--depth;

		tree += tree_prefix(depth) + "Scope End";

		return tree;
	}
	LLVM::Value Scope::code_gen()
	{
		SymbolTablePtr new_symbol_table = std::make_shared<SymbolTable>();
		new_symbol_table->parent = symbol_table;
		symbol_table = new_symbol_table;
		
		LLVM::Value last_value;

		for (StatementPtr statement : statements)
		{
			last_value = statement->code_gen();
		}

		symbol_table = symbol_table->parent;

		return last_value;
	}

	/**
	 * @brief 새 ExpressionStatement 인스턴스를 만듭니다.
	 * @param token 이 노드를 만드는데 사용된 가장 첫번째 토큰입니다.
	 * @param expression 식입니다.
	 */
	ExpressionStatement::ExpressionStatement(const Token& token, ExpressionPtr expression)
		: Statement(token), expression(expression)
	{}

	std::string ExpressionStatement::tree_gen(std::size_t depth) const
	{
		return expression->tree_gen(depth);
	}
	LLVM::Value ExpressionStatement::code_gen()
	{
		return expression->code_gen();
	}
}

namespace Dlink
{
	/**
	 * @brief 이 Type 인스턴스의 멤버를 초기화합니다.
	 * @param token 이 노드를 만드는데 사용된 가장 첫번째 토큰입니다.
	 */
	Type::Type(const Token& token)
		: token(token)
	{}
}
