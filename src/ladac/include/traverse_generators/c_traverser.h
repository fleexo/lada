#pragma once

#include <traverse_generators/base_traverser.h>
#include <sstream>
#include <ast/nodes.h>

namespace lada_traverser {
    class c_traverser : public base_traverser_callbacks, public lada_ast::generation_viewer {
        public:
        auto on_block_start() -> void override {
            _generatedCode << '{';
        }

        auto on_block_end() -> void override {
             _generatedCode << '}';
        }

        void on_function_definition(lada_ast::function_def const& definition) override {
            _generatedCode << "void " << definition.name();
            if(definition.parameter_count() == 0) {
                _generatedCode << "()";
            }
        }

        void on_function_call(lada_ast::function_call const& call) override {
            _generatedCode << call.name();

            if(call.parameter_count() == 0) {
                _generatedCode << "();";
            }
        }

        void on_function_call_parameter(lada_ast::function_call_parameter const & parameter, bool const more_follows) override {
            if(!_creatingCallParameter) {
                _generatedCode << '(';
                _creatingCallParameter = true;
            }
            std::visit(lada_ast::value_visitor{
                [&](std::string_view s) { _generatedCode << '"' << s << '"'; },
                [&](int i) { _generatedCode << i; },
                [&](double d) { _generatedCode << d; },
                [&](bool b) { _generatedCode << (b ? "true" : "false");}
            }, parameter.value());

            if(more_follows == false) {
                _creatingCallParameter = false;
                _generatedCode << ");";
            } else {
                _generatedCode << ',';
            }
        }

        void on_function_def_parameter(lada_ast::function_def_parameter const& parameter, bool const more_follows) override {
            if(!_creatingDefParameter) {
                _generatedCode << '(';
                _creatingDefParameter = true;
            }

            switch(parameter.type()) {
                case lada_ast::value_type::BOOL:
                    _generatedCode << "bool " << parameter.name();
                break;
                case lada_ast::value_type::STRING:
                    _generatedCode << "const char* " << parameter.name();
                break;
                case lada_ast::value_type::INT:
                    _generatedCode << "int " << parameter.name();
                break;
                case lada_ast::value_type::DOUBLE:
                    _generatedCode << "double " << parameter.name();
                break;
            }

            if(more_follows == false) {
                _creatingDefParameter = false;
                _generatedCode << ");";
            } else {
                _generatedCode << ',';
            }
        }

        std::string view() const {
            return _generatedCode.str();
        }

        private:
        std::stringstream _generatedCode;
        bool _creatingCallParameter{false};
        bool _creatingDefParameter{false};
    };
}