#pragma once
#include <ast/node.h>
#include <vector>
#include <ast/function_def_node.h>

namespace lada_ast {

class program : public node {
public:
    program(main_function_def&& mainFunction)
        : _mainFunction {std::move(mainFunction)}, node() {}

    node_kind kind() const override {
        return node_kind::program;
    }

    void traverse(abstract_traversal& traversal) const override {
        _mainFunction.traverse(traversal);
    }
private:
    main_function_def _mainFunction;
};

} 
