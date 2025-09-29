cmake_minimum_required(VERSION 3.20)
set(target lada_common)
project(${target} LANGUAGES CXX)

set(sources 
    lada_error.cpp
    include/lada_combinators.inl)

set(headers 
    include/lada_error.h)

add_library(${target} STATIC ${sources} ${headers})

target_include_directories(${target} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")


set_property(TARGET ${target} PROPERTY CXX_STANDARD 23)
set_property(TARGET ${target} PROPERTY CXX_STANDARD_REQUIRED ON)