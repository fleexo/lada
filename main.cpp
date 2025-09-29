#include <ladac.h>
#include <format>
#include <iostream>
#include <ast/node.h>
#include <traverse_generators/c_traverser.h>


auto print_compile_error(lada_error const& error) {
    auto const msg = std::format("compilation failed: {}", error.what());
    std::cerr << msg << '\n';
}

auto main() -> int {
    auto const source_code = R"(
    fn main() {
        print("hallo")
    }
    )";

    lada_compiler compiler;
    auto const compile_result = compiler.compile(source_code);
    if(!compile_result.has_value()) {
        print_compile_error(compile_result.error());
        return 1;
    }

    lada_traverser::c_traverser traverser;
    (*compile_result).traverse(traverser);

    std::cout << traverser.view() << '\n';

    return 0;
}