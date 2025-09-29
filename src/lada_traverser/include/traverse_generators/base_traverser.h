#pragma once
#include <abstract_traversal_callbacks.h>

namespace lada_traverser {

class base_traverser_callbacks : public lada_ast::abstract_traversal {
public:
    auto on_block_start() -> void override {
    }

    auto on_block_end() -> void override {
    }

    void on_function_definition(lada_ast::function_def const&) override {
    }

    void on_function_call(lada_ast::function_call const&) override {
    }

    void on_function_call_parameter(lada_ast::function_call_parameter const &, bool const) override {
    }

    void on_function_def_parameter(lada_ast::function_def_parameter const &, bool const) override {
    }
};

} // namespace lada_traverser
