#include "ParseStruct.hh"

namespace Dlink
{
    std::string tree_prefix(std::size_t depth)
    {
        return std::string(depth * 6, '-');
    }

    std::string Integer32::tree_gen(std::size_t depth)
    {
        return tree_prefix(depth) + "Integer32(" + std::to_string(data) + ")";
    }
    
    std::string BinaryOperation::tree_gen(std::size_t depth)
    {
        std::string tree = tree_prefix(depth)+ "BinaryOperation:\n";
        tree += tree_prefix(depth) + "lhs:\n";
        tree += lhs->tree_gen(depth+1) + '\n';
        tree += tree_prefix(depth) + "rhs:\n";
        tree += rhs->tree_gen(depth+1);

        return tree;
    }
    
    std::string UnaryOperation::tree_gen(std::size_t depth)
    {
        std::string tree = tree_prefix(depth) + "UnaryOperation:\n";
        tree += tree_prefix(depth) + "rhs:\n";
        tree += rhs->tree_gen(depth+1);

        return tree;
    }
}
