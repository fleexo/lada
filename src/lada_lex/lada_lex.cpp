#include <lada_lex.h>

#include <cctype>
#include <lada_combinators.inl>

auto lada_lex::try_get_single_token(char const rune) const -> std::optional<std::pair<lada_token_meta, size_t>> {
    lada_token mapped_token;
    switch (rune) {
        case '{': mapped_token = lada_token::CURLY_BRACES_OPEN; break;
        case '}': mapped_token = lada_token::CURLY_BRACES_CLOSE; break;
        case '=': mapped_token = lada_token::EQUALS; break;
        case ';': mapped_token = lada_token::SEMICOLON; break;
        case '(': mapped_token = lada_token::BRACKET_OPEN; break;
        case ')': mapped_token = lada_token::BRACKET_CLOSE; break;
        case ',': mapped_token = lada_token::COMMA; break;
        case ':': mapped_token = lada_token::COLON; break;
        default: return std::nullopt;
    }

    return std::make_pair(lada_token_meta {
        .Token = mapped_token,
        .Position = token_position {
            .Position = _position
        },
    }, 1);

}

auto lada_lex::try_get_keyword_token(std::string_view const& rest_data) const -> std::optional<std::pair<lada_token_meta, size_t>> {
    static constexpr std::array<std::pair<std::string_view, lada_token>, 5> KEYWORD_MAP = {
        {{"fn", lada_token::KEYWORD_FN},
        {"void", lada_token::KEYWORD_VOID},
        {"int", lada_token::KEYWORD_INT},
        {"return", lada_token::KEYWORD_RETURN},
        {"string", lada_token::KEYWORD_STRING}}
    };

    for(auto const& [keyword, token] : KEYWORD_MAP) {
        if(rest_data.starts_with(keyword)) {
            return std::make_pair(lada_token_meta {
                .Token = token,
                .Position = token_position {
                    .Position = _position
                },
            }, keyword.size());
        }
    }

    return std::nullopt;
}

auto lada_lex::try_get_number_token(std::string_view const& rest_data) const
    -> std::optional<std::pair<lada_token_meta, size_t>>
{
    size_t idx{0};
    auto const sz = rest_data.size();

    while (idx < sz && std::isdigit(static_cast<unsigned char>(rest_data[idx]))) {
        ++idx;
    }

    if (idx == 0) {
        return std::nullopt;
    }

    return std::make_pair(
        lada_token_meta{
            .Token = lada_token::NUMBER,
            .Position = token_position{.Position = _position},
            .Lexeme = rest_data.substr(0, idx)
        },
        idx
    );
}

auto lada_lex::try_get_identifier_token(std::string_view const& rest_data) const
    -> std::optional<std::pair<lada_token_meta, size_t>>
{
    size_t idx{0};
    auto const sz = rest_data.size();

    while (idx < sz && std::isalpha(static_cast<unsigned char>(rest_data[idx]))) {
        ++idx;
    }

    if (idx == 0) {
        return std::nullopt;
    }

    return std::make_pair(
        lada_token_meta{
            .Token = lada_token::IDENTIFIER,
            .Position = token_position{.Position = _position},
            .Lexeme = rest_data.substr(0, idx)
        },
        idx
    );
}

auto lada_lex::try_get_string_token(std::string_view const& rest_data) const -> std::optional<std::pair<lada_token_meta, size_t>> {
    if(!rest_data.starts_with('"')) {
        return std::nullopt;
    }

    size_t idx {1}; // we ignore the first one, since it's a quote
    auto const sz = rest_data.size();
    while(idx < sz) { // todo: escaping
        if(rest_data[idx] == '"')
        {
            return std::make_pair(
                lada_token_meta{
                    .Token = lada_token::STRING,
                    .Position = token_position{.Position = _position},
                    .Lexeme = rest_data.substr(1, idx-1)
                },
                idx
            );
        }
        idx++;
    }
    return std::nullopt;
}

auto lada_lex::lex(std::string_view const& source_code) -> std::expected<std::vector<lada_token_meta>, lada_error> {
    auto const sz = source_code.size();
    std::vector<lada_token_meta> tokens;
    tokens.reserve(source_code.size());
    while (_position < sz) {
        using TokenPredicate = std::function<std::optional<std::pair<lada_token_meta, size_t>>()>;
        
        auto const rune = source_code[_position];
        auto const rest_data = source_code.substr(_position);

        auto const token_result = lada_combinators::any<std::pair<lada_token_meta, size_t>>(
            std::array<TokenPredicate, 5>{
                [&]() { return try_get_single_token(rune); },
                [&]() { return try_get_keyword_token(rest_data); },
                [&]() { return try_get_string_token(rest_data); },
                [&]() { return try_get_number_token(rest_data); },
                [&]() { return try_get_identifier_token(rest_data); }
            }
        );

        if(token_result.has_value()) {
            auto const& [token, step] = *token_result;
            tokens.emplace_back(std::move(token));
            _position += step;
            continue;
        } else {
            _position++;
        }
    }
    return tokens;
}