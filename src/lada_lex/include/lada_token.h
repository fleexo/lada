#pragma once
#include <cstdint>
#include <stddef.h>

enum class lada_token : uint8_t {
    KEYWORD_VOID,
    KEYWORD_STRING,
    KEYWORD_INT,
    KEYWORD_BOOL,
    KEYWORD_DOUBLE,

    KEYWORD_FN,
    KEYWORD_RETURN,

    CURLY_BRACES_OPEN,
    CURLY_BRACES_CLOSE,
    BRACKET_OPEN,
    BRACKET_CLOSE,

    EQUALS,
    SEMICOLON,
    COLON,
    COMMA,

    STRING,
    NUMBER,
    IDENTIFIER,
};

inline std::string_view lada_token_to_string(lada_token tok) {
    switch (tok) {
        case lada_token::KEYWORD_VOID:    return "KEYWORD_VOID";
        case lada_token::KEYWORD_STRING:  return "KEYWORD_STRING";
        case lada_token::KEYWORD_FN:      return "KEYWORD_FN";
        case lada_token::KEYWORD_INT:    return "KEYWORD_INT";
        case lada_token::KEYWORD_RETURN:      return "KEYWORD_RETURN";
        case lada_token::CURLY_BRACES_OPEN: return "CURLY_BRACES_OPEN";
        case lada_token::CURLY_BRACES_CLOSE: return "CURLY_BRACES_CLOSE";
        case lada_token::BRACKET_OPEN:    return "BRACKET_OPEN";
        case lada_token::BRACKET_CLOSE:   return "BRACKET_CLOSE";
        case lada_token::EQUALS:          return "EQUALS";
        case lada_token::SEMICOLON:       return "SEMICOLON";
        case lada_token::STRING:          return "STRING";
        case lada_token::NUMBER:          return "NUMBER";
        case lada_token::IDENTIFIER:      return "IDENTIFIER";
        case lada_token::COLON:          return "COLON";
        case lada_token::COMMA:      return "COMMA";
    }
    return "UNKNOWN";
}

struct token_position {
    size_t const Position;
};

struct lada_token_meta {
    lada_token const Token;
    token_position const Position;
    std::optional<std::string_view> Lexeme;
};