#pragma once
#include "node.h"
#include <vector>
#include <variant>
#include <ast/function_call_node.h>
#include <ast/abstract_traversal_callbacks.h>

namespace lada_ast {

class block : public node {
public:
    using block_statement_variant = std::variant<function_call>;

    block(std::vector<block_statement_variant>&& statements)
        : _statements{std::move(statements)} {}

    node_kind kind() const override {
        return node_kind::block;
    }

    void traverse(abstract_traversal& traversal) const override {
        traversal.on_block_start();
        for (auto const& statement : _statements) {
            std::visit([&traversal](auto const& node) {
                node.traverse(traversal);
            }, statement);
        }
        traversal.on_block_end();
    }

private:
    std::vector<block_statement_variant> _statements; 
};

} 
