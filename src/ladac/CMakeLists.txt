cmake_minimum_required(VERSION 3.20)
set(target ladac)
project(${target} LANGUAGES CXX)

set(sources 
    ladac.cpp)

set(headers 
    include/ladac.h
    include/ast/nodes.h
    include/ast/node.h
    include/ast/function_def_node.h
    include/ast/function_call_node.h
    include/ast/function_call_parameter_node.h
    include/ast/block_node.h
    include/ast/abstract_traversal_callbacks.h
    include/ast/parameter_values.h
    include/ast/function_def_parameter_node.h

    include/traverse_generators/base_traverser.h
    include/traverse_generators/c_traverser.h
    )

add_library(${target} STATIC ${sources} ${headers})

target_include_directories(${target} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(${target} PUBLIC lada_common lada_lex)

set_property(TARGET ${target} PROPERTY CXX_STANDARD 23)
set_property(TARGET ${target} PROPERTY CXX_STANDARD_REQUIRED ON)