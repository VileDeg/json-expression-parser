cmake_minimum_required(VERSION 3.20)
#set(CMAKE_GENERATOR "MinGW Makefiles")
#set(CMAKE_MAKE_PROGRAM "C:/Program Files (x86)/GnuWin32/bin/make.exe")

project(json_eval)

# Enable testing
#enable_testing()

set(CMAKE_CXX_STANDARD 20)

# Optionally, set the C and C++ compilers
#set(CMAKE_C_COMPILER "C:/Strawberry/c/bin/gcc.exe")
#set(CMAKE_CXX_COMPILER "C:/Strawberry/c/bin/c++.exe")


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# include(FetchContent)
# FetchContent_Declare(
#   googletest
#   GIT_REPOSITORY https://github.com/google/googletest.git
#   GIT_TAG        b514bdc898e2951020cbdca1304b75f5950d1f59 # release-1.15.2
# )
# # For Windows: Prevent overriding the parent project's compiler/linker settings
# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# FetchContent_MakeAvailable(googletest)



set(CMAKE_BUILD_TYPE Debug)

# Set output directory for Visual Studio or Xcode
if (CMAKE_GENERATOR MATCHES "Visual Studio" OR CMAKE_GENERATOR MATCHES "Xcode")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG .)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE .)
endif()

set(SRC_DIR src)

set(SOURCES
    #${SRC_DIR}/json_types.h 
    
    #${SRC_DIR}/json_parser.h 
    ${SRC_DIR}/json_parser.cpp
)

add_executable(json_eval ${SRC_DIR}/main.cpp ${SOURCES})

target_include_directories(json_eval PRIVATE src)

# Tests
add_subdirectory(gtest)