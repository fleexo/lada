#pragma once
#include <concepts>
#include <optional>
#include <utility>

template<typename T, typename OPT_TYP>
concept TokenPredicateContainer =
    requires(T container) {
        { (*container.begin())() } -> std::same_as<std::optional<OPT_TYP>>;
    };

class lada_combinators {
    public:
    template<typename T>
    static auto any(TokenPredicateContainer<T> auto const& token_predicates)
        -> std::optional<T> 
    {
        for (const auto& predicate : token_predicates) {
            if (auto result = predicate(); result.has_value()) {
                return result;
            }
        }
        return std::nullopt;
    }
};
