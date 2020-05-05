cmake_minimum_required(VERSION 3.6)

project(vili)

add_subdirectory(extlibs/fmt)
add_subdirectory(extlibs/pegtl)

file(GLOB VILI_HEADERS include/vili/*.hpp)
file(GLOB VILI_SOURCES src/*.cpp)

add_executable(vili ${VILI_HEADERS} ${VILI_SOURCES})

target_link_libraries(vili fmt)
target_link_libraries(vili pegtl)

target_include_directories(vili
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_EXTENSIONS OFF)