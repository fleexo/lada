cmake_minimum_required(VERSION 3.20)
set(target lada_lex)
project(${target} LANGUAGES CXX)

set(sources 
    lada_lex.cpp)

set(headers 
    include/lada_lex.h
    include/lada_token.h)

add_library(${target} STATIC ${sources} ${headers})

target_include_directories(${target} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(${target} PUBLIC lada_common)

set_property(TARGET ${target} PROPERTY CXX_STANDARD 23)
set_property(TARGET ${target} PROPERTY CXX_STANDARD_REQUIRED ON)