cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(target lada_binary)
project(${target} LANGUAGES CXX)

set(sources 
        elf_writer.cpp
    )

set(headers 
    include/elf_writer.h
    include/abstract_binary_writer.h
    )

add_library(${target} STATIC ${sources} ${headers})

target_include_directories(${target} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(${target} PUBLIC elfio)

set_property(TARGET ${target} PROPERTY CXX_STANDARD 23)
set_property(TARGET ${target} PROPERTY CXX_STANDARD_REQUIRED ON)
