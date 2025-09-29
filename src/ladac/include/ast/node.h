#pragma once
#include <vector>
#include <cstdint>

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

class abstract_traversal;

class node {
public:
    node(const node&) = delete;
    node& operator=(const node&) = delete;
    node(node&&) = default;
    node() = default;
    node& operator=(node&&) = default;
    virtual ~node() = default;

    virtual node_kind kind() const = 0;
    virtual void traverse(abstract_traversal& traversal) const = 0;
};

}
