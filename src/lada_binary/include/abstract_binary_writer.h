#pragma once
#include <cstdint>
#include <span>
#include <vector>
#include <string>

namespace lada_binary {
    struct abstract_binary_writer {
        virtual auto set_machine_code(std::span<uint8_t const> const&) -> void = 0;
        virtual auto write(std::string const& filename) -> std::vector<uint8_t> = 0;
    };
}