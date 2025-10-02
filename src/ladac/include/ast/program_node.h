#pragma once
#include <ast/node.h>
#include <vector>
#include <ast/function_def_node.h>

namespace lada_ast {

class program : public node {
public:
    program(function_def&& entryFunction)
        : _entryFunction {std::move(entryFunction)}, node() {}

    node_kind kind() const override {
        return node_kind::program;
    }

    void traverse(abstract_traversal& traversal) override {
        _entryFunction.traverse(traversal);
    }
private:
    function_def _entryFunction;
};

} 
