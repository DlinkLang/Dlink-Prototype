#include "ParseStruct/Root.hh"
#include "CodeGen.hh"

namespace Dlink
{
	extern std::string tree_prefix(std::size_t depth);

	/**
	 * @brief 새 Identifier 인스턴스를 만듭니다.
	 * @param id 식별자 값입니다.
	 */
	Identifer::Identifer(const std::string& id)
		: id(id)
	{}

	std::string Identifer::tree_gen(std::size_t depth)
	{
		return tree_prefix(depth) + "Identifier(\"" + id + "\")\n";
	}
	LLVM::Value Identifer::code_gen()
	{
		LLVM::Value result = symbol_table->find(id);

		if (result == nullptr)
		{
			// TODO
			return nullptr;
		}

		return result;
	}

	/**
	 * @brief 새 Block 인스턴스를 만듭니다.
	 * @param statements Statement들의 집합입니다.
	 */
	Block::Block(const std::vector<StatementPtr>& statements)
		: statements(statements)
	{}

	std::string Block::tree_gen(std::size_t depth)
	{
		std::string tree = tree_prefix(depth) + "Block Start\n";

		for (StatementPtr statement : statements)
		{
			tree += statement->tree_gen(depth + 1);
		}

		tree += tree_prefix(depth) + "Block End\n";

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
	 * @param statements Statement들의 집합입니다.
	 * @param parent 현재 스코프의 상위 스코프입니다.
	 */
	Scope::Scope(const std::vector<StatementPtr>& statements, StatementPtr parent)
		: Block(statements), parent(parent)
	{}
	
	std::string Scope::tree_gen(std::size_t depth)
	{
		std::string tree = tree_prefix(depth) + "Scope Start\n";

		++depth;
		for (StatementPtr statement : statements)
		{
			tree += statement->tree_gen(depth);
		}
		--depth;

		tree += tree_prefix(depth) + "Scope End\n";

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
	 * @param expression 식입니다.
	 */
	ExpressionStatement::ExpressionStatement(ExpressionPtr expression)
		: expression(expression)
	{}

	std::string ExpressionStatement::tree_gen(std::size_t depth)
	{
		return expression->tree_gen(depth);
	}
	LLVM::Value ExpressionStatement::code_gen()
	{
		return expression->code_gen();
	}
}
