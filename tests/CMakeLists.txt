
project(vili_tests)

file(GLOB_RECURSE VILITESTS_HEADERS src/*.hpp)
file(GLOB_RECURSE VILITESTS_SOURCES src/*.cpp)

add_executable(vili_tests ${VILITESTS_HEADERS} ${VILITESTS_SOURCES})

target_include_directories(vili_tests
  PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

find_package(Catch2 CONFIG REQUIRED)
target_link_libraries(vili_tests PRIVATE
    vili
    Catch2::Catch2WithMain
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_EXTENSIONS OFF)
