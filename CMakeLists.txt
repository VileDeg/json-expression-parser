cmake_minimum_required(VERSION 3.10)
set(CMAKE_GENERATOR "MinGW Makefiles")
set(CMAKE_MAKE_PROGRAM "C:/Program Files (x86)/GnuWin32/bin/make.exe")

project(json_eval)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Specify the generator for MinGW Makefiles


# Optionally, set the C and C++ compilers
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

set(CMAKE_BUILD_TYPE Debug)

add_executable(json_eval main.cpp json_types.h json_parser.h json_parser.cpp)