
cmake_minimum_required(VERSION 3.23)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON
    CACHE STRING "Create a compile_commands.json file"
)

if(DEFINED "ENV{VCPKG_ROOT}" AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Try to use vcpkg.")
endif()

project(vili
    VERSION 1.0.0
    DESCRIPTION "A nice and readable data format !"
    HOMEPAGE_URL "https://github.com/ViliOrg/Vili"
    LANGUAGES "CXX"
)

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

add_library(vili)
add_library(vili::vili ALIAS vili)
target_sources(vili
    PRIVATE ${VILI_SOURCES}
    PUBLIC FILE_SET HEADERS BASE_DIRS "include" FILES ${VILI_HEADERS}
)

find_package(fmt CONFIG REQUIRED)
find_package(nlohmann-fifo-map CONFIG REQUIRED)
find_package(pegtl CONFIG REQUIRED)

target_link_libraries(vili PRIVATE
    fmt::fmt
    nlohmann-fifo-map::nlohmann-fifo-map
    taocpp::pegtl
)

set_target_properties(vili
    PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        DEBUG_POSTFIX "d"
)

if(NOT DEFINED BUILD_TESTS)
    set(BUILD_TESTS OFF CACHE BOOL "Build Vili Tests ?")
endif()

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()


# Install the library and headers.
install(TARGETS "vili"
    EXPORT "vili-target"
    FILE_SET HEADERS
)

# uninstall target
if(NOT TARGET uninstall)
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/uninstall.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
        IMMEDIATE @ONLY
    )
    add_custom_target(uninstall
        COMMAND "${CMAKE_COMMAND}" "-P" "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    )
endif()


#Add export configuration, so it can be found via find_package
install(EXPORT "vili-target"
  DESTINATION "share/vili"
  NAMESPACE "vili::"
)

include(CMakePackageConfigHelpers)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/config.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/vili-config.cmake"
    INSTALL_DESTINATION "share/vili"
)

write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/vili-config-version.cmake"
  VERSION "${PROJECT_VERSION}"
  COMPATIBILITY SameMajorVersion
)

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/vili-config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/vili-config-version.cmake"
    DESTINATION "share/vili"
)
