cmake_minimum_required(VERSION 3.20)

project(json_eval)

set(CMAKE_CXX_STANDARD 20)


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


set(CMAKE_BUILD_TYPE Debug)

# Set output directory for Visual Studio or Xcode
if (CMAKE_GENERATOR MATCHES "Visual Studio" OR CMAKE_GENERATOR MATCHES "Xcode")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG .)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE .)
endif()

set(SRC_DIR src)

set(SOURCES
    ${SRC_DIR}/json_parser.cpp
    ${SRC_DIR}/json_eval.cpp
)

add_executable(json_eval ${SRC_DIR}/main.cpp ${SOURCES})

target_include_directories(json_eval PRIVATE src)

# Tests
add_subdirectory(gtest)