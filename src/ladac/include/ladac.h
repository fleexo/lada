#pragma once

#include <string_view>
#include <expected>
#include <lada_error.h>
#include <span>
#include <ast/node.h>
#include <optional>
#include <lada_token.h>
#include <span>
#include <ast/nodes.h>

class lada_compiler {
    public:
    [[nodiscard]] auto compile(std::string_view const& source_code) -> std::expected<lada_ast::program, lada_error>;

    private:
    template<typename T>
    using parse_result = std::expected<T, lada_error>;
    using token_view = std::span<lada_token_meta const>;

    auto peek_token(size_t const n = 0) const -> std::optional<lada_token_meta const>;
    auto consume_token(size_t const n = 1) -> bool; // returns true, if it was consumed, false if end of stream was reached
    auto get_value_type(lada_token const token) const -> std::optional<lada_ast::value_type>;
    auto expect_single_token(lada_token const token, std::optional<std::string_view const> expected_lexeme = {}) const -> std::optional<lada_error>;
    auto expect_and_emit_lexeme(lada_token const token) const -> std::expected<std::string_view, lada_error>;
    auto expect_type_keyword() const -> parse_result<lada_ast::value_type>;

    auto parse_function_def() -> parse_result<lada_ast::function_def>;
    auto parse_function_def_parameter() -> parse_result<lada_ast::function_def_parameter>;
    auto parse_function_def_parameters() -> parse_result<std::vector<lada_ast::function_def_parameter>>;
    auto parse_function_call() -> parse_result<lada_ast::function_call>;
    auto parse_return() -> parse_result<lada_ast::function_return>;
    auto parse_statement() -> parse_result<lada_ast::block::block_statement_variant>;
    auto parse_block() -> parse_result<lada_ast::block>;

    #pragma region template_helper

    private:
    template <typename Pred>
    auto consume_after(Pred&& pred) {
        return [this, pred = std::forward<Pred>(pred)]() -> std::optional<lada_error> {
            if (auto err = pred()) return err;
            consume_token();
            return std::nullopt;
        };
    }

    #pragma endregion

    private:
    token_view _tokens;
    size_t _cursor {0};
};