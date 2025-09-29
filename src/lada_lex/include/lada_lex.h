#pragma once

#include <string_view>
#include <expected>
#include <optional>

#include <lada_token.h>
#include <lada_error.h>
#include <functional>
#include <array>


class lada_lex {

    public:
    [[nodiscard]] auto lex(std::string_view const& source_code) -> std::expected<std::vector<lada_token_meta>, lada_error>;

    private:

    auto try_get_single_token(char const rune) const -> std::optional<std::pair<lada_token_meta, size_t>>;
    auto try_get_keyword_token(std::string_view const& rest_data) const -> std::optional<std::pair<lada_token_meta, size_t>>;
    auto try_get_number_token(std::string_view const& rest_data) const -> std::optional<std::pair<lada_token_meta, size_t>>;
    auto try_get_string_token(std::string_view const& rest_data) const -> std::optional<std::pair<lada_token_meta, size_t>>;
    auto try_get_identifier_token(std::string_view const& rest_data) const -> std::optional<std::pair<lada_token_meta, size_t>>;

    private:
    size_t _position {0};
};