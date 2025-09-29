#pragma once
#include "node.h"
#include <vector>
#include <string>
#include <variant>
#include <abstract_traversal_callbacks.h>
#include <ast/parameter_values.h>

namespace lada_ast {

class function_def_parameter : public node {
public:

    function_def_parameter(std::string_view parameter_name, value_type const value_type)
        : _name{parameter_name}, _type{value_type}, node{} {}

    const value_type& type() const { return _type; }
    const std::string_view& name() const { return _name; }

    node_kind kind() const override { return node_kind::parameter; }

    void traverse(abstract_traversal& traversal) const override {
        traversal.on_function_def_parameter(*this, false);
    }

private:
    value_type const _type;
    std::string_view const _name;
};


}
