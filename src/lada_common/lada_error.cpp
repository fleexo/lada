#include <lada_error.h>

lada_error::lada_error(std::string const& msg): _msg {std::move(msg)} {}

auto lada_error::what() const -> std::string_view {
    return _msg;
}