
cmake_minimum_required(VERSION 3.23)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON
    CACHE STRING "Create a compile_commands.json file"
)

if(DEFINED "ENV{VCPKG_ROOT}" AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Try to use vcpkg.")
endif()

project(vili)

set(VILI_HEADERS
    include/vili/config.hpp
    include/vili/exceptions.hpp
    include/vili/node.hpp
    include/vili/parser.hpp
    include/vili/types.hpp
    include/vili/utils.hpp
    include/vili/writer.hpp
    include/vili/parser/actions.hpp
    include/vili/parser/grammar.hpp
    include/vili/parser/grammar_errors.hpp
    include/vili/parser/parser_state.hpp
)
set(VILI_SOURCES
    src/node.cpp
    src/parser.cpp
    src/types.cpp
    src/utils.cpp
    src/writer.cpp
    src/parser/parser_state.cpp
)

add_library(vili ${VILI_HEADERS} ${VILI_SOURCES})

find_package(fmt CONFIG REQUIRED)
find_package(nlohmann-fifo-map CONFIG REQUIRED)
find_package(pegtl CONFIG REQUIRED)

target_link_libraries(vili PRIVATE
    fmt::fmt
    nlohmann-fifo-map::nlohmann-fifo-map
    taocpp::pegtl
)

target_include_directories(vili
    PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_EXTENSIONS OFF)

if(NOT DEFINED BUILD_TESTS)
    set(BUILD_TESTS OFF CACHE BOOL "Build Vili Tests ?")
endif()

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
