#include <memory>
#include <string_view>
#include <vector>
#include "node.h"

namespace lada_ast {
class function_call : public node {
public:
    function_call(std::string_view name,
                  std::vector<std::unique_ptr<node>> parameters)
        : node(std::move(parameters)), _name(name) {}

    node_kind kind() const override {
        return node_kind::function_call;
    }

    const std::string_view& name() const { return _name; }

private:
    std::string_view _name;
};
}