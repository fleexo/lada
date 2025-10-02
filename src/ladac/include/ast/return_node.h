#pragma once
#include <ast/function_call_parameter_node.h>

namespace lada_ast {
    class function_return : public function_call_parameter {
        public:
        explicit function_return(value_variant&& value)
            : function_call_parameter(std::move(value)) {}

        node_kind kind() const override { return node_kind::function_return; }

        void traverse(abstract_traversal& traversal) override {
            traversal.on_function_return(*this);
        }
    };

}