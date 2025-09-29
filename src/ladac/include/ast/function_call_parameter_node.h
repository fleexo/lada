#pragma once
#include "node.h"
#include <vector>
#include <string>
#include <variant>

namespace lada_ast {

class function_call_parameter : public node {
public:
    using value_variant = std::variant<std::string_view, int, double, bool>;

    explicit function_call_parameter(value_variant value)
        : _value{std::move(value)} {}

    const value_variant& value() const { return _value; }

    node_kind kind() const override { return node_kind::parameter; }

private:
    value_variant _value;
};


}
