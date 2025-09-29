cmake_minimum_required(VERSION 3.20)
set(target lada)
project(${target})

add_subdirectory(src)
add_subdirectory(third-party/asmjit)

set(sources
    main.cpp)

add_executable(${target} ${sources})

target_link_libraries(${target} ladac)

set_property(TARGET ${target} PROPERTY CXX_STANDARD 23)
set_property(TARGET ${target} PROPERTY CXX_STANDARD_REQUIRED ON)