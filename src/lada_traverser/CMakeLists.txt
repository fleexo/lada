cmake_minimum_required(VERSION 3.20)
set(target lada_traverser)
project(${target} LANGUAGES CXX)

set(sources )

set(headers 
    include/traverse_generators/base_traverser.h
    include/traverse_generators/asmjit_traverser.h
    )

add_library(${target} STATIC ${sources} ${headers})

target_include_directories(${target} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(${target} PUBLIC lada_common ladac asmjit)

set_property(TARGET ${target} PROPERTY CXX_STANDARD 23)
set_property(TARGET ${target} PROPERTY CXX_STANDARD_REQUIRED ON)