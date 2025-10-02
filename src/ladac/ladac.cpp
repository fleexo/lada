#include <ladac.h>
#include <lada_lex.h>
#include <format>

#include <lada_combinators.inl>


auto lada_compiler::peek_token(size_t const n) const -> std::optional<lada_token_meta const> {
    auto const peek_index {n + _cursor};
    if(peek_index < _tokens.size()) {
        return _tokens[peek_index];
    }

    return std::nullopt;
}
auto lada_compiler::consume_token(size_t const n) -> bool {
    auto const peek_index {n + _cursor};
    if(_tokens.size() < peek_index) {
        return false;
    }

    _cursor = peek_index;
    return true;
}

auto lada_compiler::expect_single_token(lada_token const token, std::optional<std::string_view const> lexeme) const -> std::optional<lada_error> {
    if(auto const token_result = peek_token(0); !token_result.has_value()) {
        return lada_error{std::format("expected token '{}', but reached EOF.", lada_token_to_string(token))};
    } else if (auto const token_value = *token_result; token_value.Token != token) {
        return lada_error{std::format("expected token '{}', but got token '{}' at position '{}'.", lada_token_to_string(token), lada_token_to_string(token_value.Token), token_value.Position.Position)};
    } else if(lexeme.has_value()) {
        if(!token_value.Lexeme.has_value()) {
            return lada_error{std::format("expected token '{}' with lexeme '{}', but got no lexeme at position '{}'.", 
                static_cast<uint8_t>(token), *lexeme, token_value.Position.Position)};
        } else if(*token_value.Lexeme != *lexeme) {
            return lada_error{std::format("expected token '{}' with lexeme '{}', but got lexeme '{}' at position '{}'.", 
                static_cast<uint8_t>(token), *lexeme, *token_value.Lexeme, token_value.Position.Position)};
        }
    }

    return std::nullopt; // this is the success case.
}
auto lada_compiler::expect_and_emit_lexeme(lada_token const token) const -> std::expected<std::string_view, lada_error> {
    if(auto const token_result = expect_single_token(token); token_result.has_value()) {
        return std::unexpected(*token_result);
    }
    if(auto const token_lexeme = (*peek_token()).Lexeme ; token_lexeme.has_value() == false) {
        return std::unexpected(lada_error{std::format("could not get the lexeme name of the token '{}' at position '{}'", lada_token_to_string(token), (*peek_token()).Position.Position)});
    } else {
        return *token_lexeme;
    }
}

auto lada_compiler::get_value_type(lada_token const token) const -> std::optional<lada_ast::value_type> {
    switch(token) {
        case lada_token::KEYWORD_DOUBLE: return lada_ast::value_type::DOUBLE;
        case lada_token::KEYWORD_BOOL: return lada_ast::value_type::BOOL;
        case lada_token::KEYWORD_STRING: return lada_ast::value_type::STRING;
        case lada_token::KEYWORD_INT: return lada_ast::value_type::INT;
        default: return {};
    }
}

auto lada_compiler::expect_type_keyword() const -> parse_result<lada_ast::value_type> {
    using Predicate = std::function<std::optional<lada_error>()>;

    auto const actual_token_opt {peek_token()};
    if(!actual_token_opt.has_value()) {
        return std::unexpected(lada_error{"expected token value type specifier, but reached EOF."});
    }

    if(auto const value_type_result = get_value_type(actual_token_opt->Token); value_type_result.has_value()) {
        return *value_type_result;
    }

    return std::unexpected{lada_error{std::format("expected value type specifier, at position '{}', but got '{}'",
        actual_token_opt->Position.Position, lada_token_to_string(actual_token_opt->Token))}};
}

auto lada_compiler::parse_function_call() -> parse_result<lada_ast::function_call> {
    std::string_view function_name;
    if(auto const token_result = expect_and_emit_lexeme(lada_token::IDENTIFIER); !token_result.has_value()) {
        return std::unexpected(token_result.error());
    } else {
        function_name = *token_result;
        consume_token();
    }

    if(auto const token_result = consume_after([&]{ return expect_single_token(lada_token::BRACKET_OPEN); })(); 
            token_result.has_value()) {
        return std::unexpected(*token_result);
    }
    std::vector<lada_ast::function_call_parameter> parameters;

    int parameter;
    if(auto const token_result = expect_and_emit_lexeme(lada_token::NUMBER); !token_result.has_value()) {
        return std::unexpected(token_result.error());
    } else {
        parameter = std::atoi((*token_result).data());
        consume_token();
    }

    parameters.emplace_back(parameter);

    if(auto const token_result = consume_after([&]{ return expect_single_token(lada_token::BRACKET_CLOSE); })(); 
        token_result.has_value()) {
        return std::unexpected(*token_result);
    }

    return lada_ast::function_call{function_name, std::move(parameters)};
}

auto lada_compiler::parse_return() -> parse_result<lada_ast::function_return> {
    if(auto const token_result = consume_after([&]{ return expect_single_token(lada_token::KEYWORD_RETURN); })(); 
        token_result.has_value()) {
        return std::unexpected(*token_result);
    }

    int return_value;
    if(auto const token_result = expect_and_emit_lexeme(lada_token::NUMBER); !token_result.has_value()) {
        return std::unexpected(token_result.error());
    } else {
        return_value = std::atoi((*token_result).data());
        consume_token();
    }

    return lada_ast::function_return{std::move(return_value)};
}

auto lada_compiler::parse_statement()
    -> parse_result<lada_ast::block::block_statement_variant>
{
    if (auto const end_statement = expect_single_token(lada_token::CURLY_BRACES_CLOSE);
        !end_statement.has_value())
    {
        return std::monostate{};
    }

    if (auto token_res = parse_return(); token_res.has_value()) {
        auto variant = lada_ast::block::block_statement_variant{
            std::in_place_type<lada_ast::function_return>, 
            std::move(*token_res)
        };
         return variant;
    }

    if (auto token_res = parse_function_call(); token_res.has_value()) {
        auto variant = lada_ast::block::block_statement_variant{
            std::in_place_type<lada_ast::function_call>, 
            std::move(*token_res)
        };
         return variant;
    }

    return std::unexpected(lada_error{"Unknown statement type"});
}


auto lada_compiler::parse_block() -> parse_result<lada_ast::block> {

    using Predicate = std::function<std::optional<lada_error>()>;
    
    if(auto const token_result = consume_after([&]{ return expect_single_token(lada_token::CURLY_BRACES_OPEN); })(); 
            token_result.has_value()) {
        return std::unexpected(*token_result);
    }


    std::vector<lada_ast::block::block_statement_variant> statements;
    while (true) {
        auto statement = parse_statement();
        if (!statement.has_value()) break;
        if (std::holds_alternative<std::monostate>(statement.value())) {
            break;
        }
        statements.emplace_back(std::move(statement.value()));
    }


    if(auto const token_result = consume_after([&]{ return expect_single_token(lada_token::CURLY_BRACES_CLOSE); })(); 
            token_result.has_value()) {
        return std::unexpected(*token_result);
    }

    return lada_ast::block{std::move(statements)};
}

auto lada_compiler::parse_function_def_parameter() -> parse_result<lada_ast::function_def_parameter> {
    std::string_view parameter_name;
    if(auto const token_result = expect_and_emit_lexeme(lada_token::IDENTIFIER); !token_result.has_value()) {
        return std::unexpected(token_result.error());
    } else {
        parameter_name = *token_result;
        consume_token();
    }

    if(auto const colon_result = consume_after([&]{ return expect_single_token(lada_token::COLON); })(); 
        colon_result.has_value()) {
        return std::unexpected(*colon_result);
    }

    if(auto const type_result = expect_type_keyword(); 
        !type_result.has_value()) {
        return std::unexpected(type_result.error());
    } else {
        consume_token();
        return lada_ast::function_def_parameter{std::move(parameter_name), *type_result};
    }
}

auto lada_compiler::parse_function_def_parameters() -> parse_result<std::vector<lada_ast::function_def_parameter>> {
    std::vector<lada_ast::function_def_parameter> parameters;
    auto current_token_opt = peek_token();
    if(!current_token_opt.has_value()) {
        return std::unexpected(lada_error{std::format("expected token '{}', but reached EOF.", lada_token_to_string(lada_token::BRACKET_CLOSE))});
    }

    if(current_token_opt->Token == lada_token::BRACKET_CLOSE) {
        return {};
    }

    while(true) {
        if(auto def_parameter_result = parse_function_def_parameter(); !def_parameter_result.has_value()) {
            return std::unexpected(def_parameter_result.error());
        } else {
            parameters.emplace_back(std::move(*def_parameter_result));

            auto current_token_opt = peek_token();
            if(!current_token_opt.has_value()) {
                return std::unexpected(lada_error{std::format("expected token '{}', but reached EOF.", lada_token_to_string(lada_token::BRACKET_CLOSE))});
            }

            if(current_token_opt->Token == lada_token::BRACKET_CLOSE) {
                return parameters;
            }

            if(auto const comma_result = consume_after([&]{ return expect_single_token(lada_token::COMMA); })(); 
                comma_result.has_value()) {
                return std::unexpected(*comma_result);
            }
        }
    }
}

auto lada_compiler::parse_function_def() -> parse_result<lada_ast::function_def> {

    if(auto const fn_result = consume_after([&]{ return expect_single_token(lada_token::KEYWORD_FN); })(); 
        fn_result.has_value()) {
        return std::unexpected(*fn_result);
    }

    std::string_view function_name;
    if(auto token_result = expect_and_emit_lexeme(lada_token::IDENTIFIER); !token_result.has_value()) {
        return std::unexpected(token_result.error());
    } else {
        function_name = std::move(*token_result);
        consume_token();
    }


    if(auto const bracket_open = consume_after([&]{ return expect_single_token(lada_token::BRACKET_OPEN); })(); 
        bracket_open.has_value()) {
        return std::unexpected(*bracket_open);
    }

    std::vector<lada_ast::function_def_parameter> parameters;
    if(auto param_res = parse_function_def_parameters(); !param_res.has_value()) {
        return std::unexpected(param_res.error());
    } else {
        parameters = std::move(*param_res);
    }

    if(auto const bracket_close = consume_after([&]{ return expect_single_token(lada_token::BRACKET_CLOSE); })(); 
        bracket_close.has_value()) {
        return std::unexpected(*bracket_close);
    }

    auto block = parse_block();
    if(!block.has_value()) {
        return std::unexpected(block.error());
    }
    return lada_ast::function_def{std::move(function_name), std::move(parameters), std::move(*block)};
}


auto lada_compiler::compile(std::string_view const& source_code) -> std::expected<lada_ast::program, lada_error> {
    lada_lex lexer;
    auto const lex_result = lexer.lex(source_code);

    if(!lex_result.has_value()) {
        return std::unexpected(lex_result.error());
    }
    
    _tokens = *lex_result;
    return parse_function_def();
}