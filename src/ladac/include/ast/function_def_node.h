#pragma once
#include "node.h"
#include <vector>
#include <string_view>
#include <ast/block_node.h>
#include <abstract_traversal_callbacks.h>
#include <ast/function_def_parameter_node.h>

namespace lada_ast {

class function_def : public node {
public:
    function_def(std::string_view&& name,
                std::vector<function_def_parameter>&& params,
                block&& body)
        : _functionBody{std::move(body)},
        _name{std::move(name)},
        _params{std::move(params)} {}

    node_kind kind() const override { return node_kind::function_def; }

    void traverse(abstract_traversal& traversal) const override {
        traversal.on_function_definition(*this);
        for(auto const& param : _params) {
            param.traverse(traversal);
        }
        _functionBody.traverse(traversal);
    }

    size_t parameter_count() const {
        return _params.size();
    }

    std::string_view const& name() const { return _name; }

private:
    std::string_view _name;
    std::vector<function_def_parameter> _params;
    block _functionBody;
};

class main_function_def : public function_def {
    public:
    inline static constexpr auto MAIN_FUNCTION_NAME = "main";
    main_function_def(block&& body) : function_def {MAIN_FUNCTION_NAME, {}, std::move(body)} {
    };
};

}
