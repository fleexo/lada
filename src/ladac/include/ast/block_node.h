#pragma once
#include "node.h"
#include <vector>
#include <variant>
#include <ast/function_call_node.h>
#include <ast/return_node.h>
#include <abstract_traversal_callbacks.h>

namespace lada_ast {

class block : public node {
public:
    using block_statement_variant = std::variant<function_call, function_return, std::monostate>;

    block(std::vector<block_statement_variant>&& statements)
        : _statements{std::move(statements)} {}

    node_kind kind() const override {
        return node_kind::block;
    }

    void traverse(abstract_traversal& traversal) override {
        traversal.on_block_start();
        for (auto& statement : _statements) {
            std::visit([&traversal](auto& node) {
            if constexpr (std::is_same_v<std::decay_t<decltype(node)>, std::monostate>) {
                return;
            } else {
                node.traverse(traversal);
            }
            }, statement);
        }
        traversal.on_block_end();
    }

private:
    std::vector<block_statement_variant> _statements; 
};

} 
