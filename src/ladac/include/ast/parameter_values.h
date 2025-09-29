#pragma once

#include <variant>
#include <string_view>

namespace lada_ast {
    using value_variant = std::variant<std::string_view, int, double, bool>;
    template <
    typename StringCallback,
    typename IntCallback,
    typename DoubleCallback,
    typename BoolCallback
    >
    struct value_visitor {
        StringCallback string_cb;
        IntCallback int_cb;
        DoubleCallback double_cb;
        BoolCallback bool_cb;

        void operator()(std::string_view s) const {
            string_cb(s);
        }

        void operator()(int i) const {
            int_cb(i);
        }

        void operator()(double d) const {
            double_cb(d);
        }

        void operator()(bool b) const {
            bool_cb(b);
        }
    };

    enum class value_type {
        STRING,
        INT,
        DOUBLE,
        BOOL
    };
}