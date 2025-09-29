#pragma once
#include <memory>
#include <vector>
#include <iostream>

namespace lada_ast {

enum class node_kind : uint8_t {
    function_def,
    function_call,
    parameter,
    block,
    program,
    identifier,
    expression,
};

class node {
public:
    virtual ~node() = default;

    virtual node_kind kind() const = 0;

    const std::vector<std::unique_ptr<node>>& children() const {
        return _children;
    }

protected:
    explicit node(std::vector<std::unique_ptr<node>> children = {})
        : _children(std::move(children)) {}

    std::vector<std::unique_ptr<node>> _children;
};

inline void print_ast(const node* n, int indent = 0) {
    auto print_indent = [&]() {
        for (int i = 0; i < indent; ++i) std::cout << "  ";
    };

    print_indent();
    switch (n->kind()) {
        case node_kind::function_def:  std::cout << "FunctionDef"; break;
        case node_kind::function_call: std::cout << "FunctionCall"; break;
        case node_kind::parameter:     std::cout << "Parameter"; break;
        case node_kind::block:         std::cout << "Block"; break;
        case node_kind::program:       std::cout << "Program"; break;
        case node_kind::identifier:    std::cout << "Identifier"; break;
        case node_kind::expression:    std::cout << "Expression"; break;
        default:                        std::cout << "UnknownNode"; break;
    }
    std::cout << "\n";

    for (const auto& child : n->children()) {
        print_ast(child.get(), indent + 1);
    }
}

}
