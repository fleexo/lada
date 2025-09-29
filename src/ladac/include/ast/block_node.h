#pragma once
#include "node.h"
#include <vector>
#include <memory>

namespace lada_ast {

class block_node : public node {
public:
    explicit block_node(std::vector<std::unique_ptr<node>> statements = {})
        : node(std::move(statements)) {}

    node_kind kind() const override {
        return node_kind::block;
    }
};

} 
