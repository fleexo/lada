#pragma once
#include "node.h"
#include <vector>
#include <string>
#include <variant>
#include <abstract_traversal_callbacks.h>
#include <ast/parameter_values.h>

namespace lada_ast {

class function_call_parameter : public node {
public:

    explicit function_call_parameter(value_variant&& value)
        : _value{std::move(value)}, node{} {}

    const value_variant& value() const { return _value; }

    node_kind kind() const override { return node_kind::parameter; }

    void traverse(abstract_traversal& traversal) const override {
        traversal.on_function_call_parameter(*this, false);
    }

private:
    value_variant _value;
};


}
