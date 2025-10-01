cmake_minimum_required(VERSION 3.20)
set(target elfio)
project(${target} LANGUAGES CXX)

set(sources )

set(headers 
elfio/elfio_dynamic.hpp  elfio/elfio_modinfo.hpp     elfio/elfio_section.hpp  elfio/elfio_symbols.hpp  elfio/elfio_versym.hpp
elfio/elfio_array.hpp  elfio/elfio_header.hpp   elfio/elfio_note.hpp        elfio/elfio_segment.hpp  elfio/elfio_utils.hpp    elfio/elf_types.hpp
elfio/elfio_dump.hpp   elfio/elfio.hpp          elfio/elfio_relocation.hpp  elfio/elfio_strings.hpp  elfio/elfio_version.hpp
    )

add_library(${target} STATIC ${sources} ${headers})

target_include_directories(${target} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

set_property(TARGET ${target} PROPERTY CXX_STANDARD 23)
set_property(TARGET ${target} PROPERTY CXX_STANDARD_REQUIRED ON)
set_target_properties(${target} PROPERTIES LINKER_LANGUAGE CXX)