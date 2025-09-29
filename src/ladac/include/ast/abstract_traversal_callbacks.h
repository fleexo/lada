#pragma once

namespace lada_ast {
    struct abstract_block_traversal {
        virtual auto on_block_start() -> void = 0;
        virtual auto on_block_end() -> void = 0;
    };


    class function_def;
    class function_call;
    class function_call_parameter;
    class function_def_parameter;
    struct abstract_function_traversal {
        virtual void on_function_definition(function_def const&) = 0;
        virtual void on_function_call(function_call const&) = 0;
        virtual void on_function_call_parameter(function_call_parameter const&, bool const more_follows) = 0;
        virtual void on_function_def_parameter(function_def_parameter const&, bool const more_follows) = 0;
    };

    struct abstract_traversal : 
        public abstract_block_traversal,
        public abstract_function_traversal {

    };

    struct generation_viewer {
        virtual std::string view() const = 0;
    };
}