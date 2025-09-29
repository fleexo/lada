#pragma once
#include "node.h"
#include <vector>
#include <string>
#include <memory>

namespace lada_ast {

class function_def : public node {
public:
    function_def(std::string name,
                std::vector<std::string> params,
                std::vector<std::unique_ptr<node>> body)
        : node(std::move(body)),
        _name(std::move(name)),
        _params(std::move(params)) {}

    node_kind kind() const override { return node_kind::function_def; }

    const std::string& name() const { return _name; }
    const std::vector<std::string>& params() const { return _params; }

private:
    std::string _name;
    std::vector<std::string> _params;
};

}
