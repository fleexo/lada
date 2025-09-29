#pragma once

#include <traverse_generators/base_traverser.h>
#include <sstream>
#include <ast/nodes.h>
#include <map>
#include <set>
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
        }
    }

    auto on_block_start() -> void override {
    }

    auto on_block_end() -> void override {
    }

    void on_function_definition(lada_ast::function_def const& definition) override {
    }

    void on_function_call(lada_ast::function_call const& call) override {
    }

    void on_function_call_parameter(lada_ast::function_call_parameter const& parameter, bool const more_follows) override {
        // Implement parameter loading here if needed
    }

    void on_function_def_parameter(lada_ast::function_def_parameter const& parameter, bool const more_follows) override {
        // Implement parameter setup here if needed
    }

    std::string view() const override {
        // Could dump generated assembly or IR
        return "";
    }

auto run() -> uint8_t override {
    asmjit::JitRuntime runtime;
    asmjit::CodeHolder code;
    code.init(runtime.environment());

    asmjit::x86::Assembler a(&code);

    // Function prologue
    a.push(asmjit::x86::rbp);
    a.mov(asmjit::x86::rbp, asmjit::x86::rsp);

    // First argument to printf (System V ABI: RDI)
    const char* msg = "BLABLA";
    a.mov(asmjit::x86::rdi, (uint64_t)msg);

    // Call printf
    void* printf_ptr = reinterpret_cast<void*>(+::printf);
    a.call(asmjit::imm(printf_ptr));

    // Function epilogue
    a.mov(asmjit::x86::rsp, asmjit::x86::rbp);
    a.pop(asmjit::x86::rbp);
    a.ret();

    // Add the generated function to the runtime
    using Fn = void (*)();
    Fn fn;
    auto err = runtime.add(&fn, &code);
    if (err != asmjit::kErrorOk) {
        throw std::runtime_error{"Failed to add JIT function"};
    }

    // Call the generated function
    fn();

    // Release JIT memory
    runtime.release(fn);

    return 0;
}

    

private:
    asmjit::JitRuntime _runtime;
    asmjit::CodeHolder _code;
    std::optional<asmjit::x86::Assembler> _assembler;

    bool _creatingCallParameter{false};
    bool _creatingDefParameter{false};
};

} // namespace lada_traverser
