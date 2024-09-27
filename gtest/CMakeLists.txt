# Tests
find_package(GTest REQUIRED)


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(TEST_DIR .)

set(TEST_SOURCES
    ../${SRC_DIR}/json_parser.cpp

    ${TEST_DIR}/parser_test.cpp
    ${TEST_DIR}/gtest_main.cpp
)

set(TEST_TARGET run_tests)


add_executable(${TEST_TARGET} ${TEST_SOURCES})


target_include_directories(${TEST_TARGET} PRIVATE ../src)


target_link_libraries(${TEST_TARGET} 
    GTest::gtest
)

enable_testing()

add_test(NAME ${TEST_TARGET} COMMAND ${TEST_TARGET})