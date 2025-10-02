#pragma once
#include <memory>
#include <string_view>
#include <vector>
#include <ast/function_call_parameter_node.h>
#include <abstract_traversal_callbacks.h>

namespace lada_ast {
class function_call : public node {
public:

    function_call(std::string_view name,
                  std::vector<function_call_parameter>&& parameters)
        : _parameters{std::move(parameters)}, _name{name} {}

    node_kind kind() const override {
        return node_kind::function_call;
    }

    void traverse(abstract_traversal& traversal) override {
        for(size_t i = 0; i < _parameters.size(); ++i) { // first traverse function parameters
            if(i == _parameters.size() - 1) {
                _parameters[i].set_last();
            }
            _parameters[i].traverse(traversal);
        }
        traversal.on_function_call(*this);
    }

    std::string_view const& name() const { return _name; }
    size_t parameter_count() const {
        return _parameters.size();
    }

private:
    std::string_view _name;
    std::vector<function_call_parameter> _parameters;
};
}