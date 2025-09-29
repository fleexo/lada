#pragma once
#include <string>
#include <string_view>

struct lada_error {
    lada_error(std::string const& msg);

    auto what() const -> std::string_view;
    private:
    std::string const _msg;
};
