#pragma once
#include <span>
#include <traverse_generators/base_traverser.h>
#include <sstream>
#include <ast/nodes.h>
#include <map>
#include <unordered_set>
#include <optional>
#include <asmjit/host.h>

#include <cstdio>

namespace lada_traverser {

class asmjit_traverser : public base_traverser_callbacks, public lada_ast::generation_viewer, public lada_ast::runner {
public:
    asmjit_traverser() {
        auto error = _code.init(_runtime.environment());
        if (error != asmjit::Error::kOk) {
            throw std::runtime_error{"could not initialize asmjit code environment: " + std::to_string(static_cast<uint64_t>(error))};
        }
        if (!_assembler.has_value()) {
            _assembler.emplace(&_code);
            _code.set_logger(&logger);
        }
    }

    auto on_block_start() -> void override {
    }

    auto on_block_end() -> void override {
    }

    void on_function_definition(lada_ast::function_def const& definition) override { 
        asmjit::Label label = _assembler->new_named_label(definition.name().data());
        _assembler->bind(label);
        _labels.emplace_back(std::move(label));
        _assembler->push(asmjit::x86::rbp);
        _assembler->mov(asmjit::x86::rbp, asmjit::x86::rsp);
    }

    void on_function_call(lada_ast::function_call const& call) override {
    }

    void on_function_return(lada_ast::function_return const& return_node) override {
        // std::visit(
        //         lada_ast::value_visitor{
        //             // String return
        //             [&](std::string_view s) {
        //             },
        //             [&](int i) {
        //                 _assembler->mov(asmjit::x86::rax, i);
        //             },
        //             [&](double d) {
        //             },
        //             [&](bool b) {
        //                 _assembler->mov(asmjit::x86::rax, static_cast<int>(b));
        //             }
        //         },
        //         return_node.value()
        // );
        // _assembler->pop(asmjit::x86::rbp);
        // _assembler->ret();

            std::visit(
        lada_ast::value_visitor{
            [&](std::string_view) {},
            [&](int i) {
                // exit(i)
                _assembler->mov(asmjit::x86::rax, 60);      // sys_exit
                _assembler->mov(asmjit::x86::rdi, i);       // exit code
                _assembler->syscall();
            },
            [&](double) {},
            [&](bool b) {
                _assembler->mov(asmjit::x86::rax, 60);
                _assembler->mov(asmjit::x86::rdi, static_cast<int>(b));
                _assembler->syscall();
            }
        },
        return_node.value()
    );
        } 

    void on_function_call_parameter(lada_ast::function_call_parameter const& parameter, bool const more_follows) override {
        // Implement parameter loading here if needed
    }

    void on_function_def_parameter(lada_ast::function_def_parameter const& parameter, bool const more_follows) override {
        // Implement parameter setup here if needed
    }

std::span<uint8_t const> view() override {

    constexpr uint64_t CODE_BASE = 0x00401000;
    _code.relocate_to_base(CODE_BASE);

    auto* section = _code.section_by_id(0);
    if (!section) return {};

    const auto& buffer = section->buffer();

    {
        std::cout << "=== ASM Output ===\n";
        std::cout << logger.data() << "\n";
    }

    return std::span<uint8_t const>(buffer.data(), buffer.size());
}

auto run() -> uint8_t override {
    using FuncType = uint8_t(*)(); // Function pointer type with no args returning uint8_t

    FuncType fn = nullptr;
    asmjit::Error err = _runtime.add(&fn, &_code);
    if (err != asmjit::kErrorOk) {
        std::cerr << "Failed to add JIT code: " << static_cast<uint32_t>(err) << "\n";
        return 0;
    }

    uint8_t result = fn(); // Call the compiled function

    _runtime.release(fn); // Free JIT memory

    return result;
}

    

private:
    asmjit::JitRuntime _runtime;
                asmjit::StringLogger logger;
    asmjit::CodeHolder _code;
    std::optional<asmjit::x86::Assembler> _assembler;

    std::vector<asmjit::Label> _labels;

    bool _creatingCallParameter{false};
    bool _creatingDefParameter{false};
};

} // namespace lada_traverser
