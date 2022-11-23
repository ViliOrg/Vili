
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

if(PROJECT_IS_TOP_LEVEL)
    #Make sure that build outputs are in predictable directories
    #This is to ensure that the DLLs are in the same directory as the test, so the tests can run
    get_property(IS_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
    if(IS_MULTI_CONFIG)
        foreach(CONFIG_TYPE IN ITEMS ${CMAKE_CONFIGURATION_TYPES})
            string(TOUPPER "${CONFIG_TYPE}" UPPERCASE_CONFIG_TYPE)
            set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${UPPERCASE_CONFIG_TYPE} "${CMAKE_BINARY_DIR}/bin-${CONFIG_TYPE}")
            set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${UPPERCASE_CONFIG_TYPE} "${CMAKE_BINARY_DIR}/lib-${CONFIG_TYPE}")
            set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${UPPERCASE_CONFIG_TYPE} "${CMAKE_BINARY_DIR}/lib-${CONFIG_TYPE}")
        endforeach()
    else()
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
    endif()
endif()


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

target_link_libraries(vili
    PUBLIC fmt::fmt
    PUBLIC nlohmann-fifo-map::nlohmann-fifo-map
    PUBLIC taocpp::pegtl
)

set_target_properties(vili
    PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        DEBUG_POSTFIX "d"
        VERSION "${PROJECT_VERSION}"
        SOVERSION "${PROJECT_VERSION_MAJOR}"
)

option(BUILD_SHARED_LIBS "Build Vili as a dynamic library?" OFF)
if(BUILD_SHARED_LIBS)
    set_target_properties(vili
        PROPERTIES
            WINDOWS_EXPORT_ALL_SYMBOLS ON
    )
endif()

option(BUILD_TESTS "Build Vili Tests?" OFF)
if(BUILD_TESTS AND PROJECT_IS_TOP_LEVEL)
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
