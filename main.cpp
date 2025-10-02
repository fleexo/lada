#include <ladac.h>
#include <format>
#include <iostream>
#include <ast/node.h>
#include <traverse_generators/asmjit_traverser.h>
#include <elf_writer.h>
#include <fstream>

auto print_compile_error(lada_error const& error) {
    auto const msg = std::format("compilation failed: {}", error.what());
    std::cerr << msg << '\n';
}

auto main() -> int {
    auto const source_code = R"(
    fn main() {
        exit(100)
    }

    )";

    lada_compiler compiler;
    auto compile_result = compiler.compile(source_code);
    if(!compile_result.has_value()) {
        print_compile_error(compile_result.error());
        return 1;
    }

    lada_traverser::asmjit_traverser traverser;
    (*compile_result).traverse(traverser);

    lada_binary::elf_writer elfWriter;
    elfWriter.set_machine_code(traverser.view());
    
    auto const binary_data = elfWriter.write("hello_x86_64");
    //std::cerr << "!: " << (int)traverser.run() << '\n';
    return 0;
}