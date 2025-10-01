#pragma once
#include <abstract_binary_writer.h>

namespace lada_binary {
    class elf_writer : public abstract_binary_writer {
        public:
        auto set_machine_code(std::span<uint8_t const> const& machineCode) -> void override;
        auto write(std::string const& filename = "") -> std::vector<uint8_t> override;

        private:
        std::span<uint8_t const> _machineCode;
    };
}