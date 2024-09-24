#include <iostream>
#include <fstream>
#include <memory>
#include <sstream>

#include "json_parser.h"

int main(int argc, char* argv[]) {
    std::cout << "Running...\n";

    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <json_file> <expression>" << std::endl;
        return 1;
    }

    {
        std::ifstream json_file(argv[1]);
        if (!json_file.is_open()) {
            std::cerr << "Error: Could not open file " << argv[1] << std::endl;
            return 1;
        }

        
        std::stringstream buffer;
        buffer << json_file.rdbuf();
        std::string json_content = buffer.str();
        std::cout << "JSON Content:\n" << json_content << std::endl;

        json_file.close();

    }

    std::ifstream json_file(argv[1]);
    if (!json_file.is_open()) {
        std::cerr << "Error: Could not open file " << argv[1] << std::endl;
        return 1;
    }

    JsonParser parser;

    std::shared_ptr<JsonValue> root;

    try {
        root = parser.Parse(json_file);
        std::cout << "[JSON parser] success." << std::endl;
    } catch (const std::runtime_error& e) {
        std::cerr << "[JSON parser] Runtime error: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "[JSON parser] Exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "[JSON parser] An unknown error occurred." << std::endl;
        return 1;
    }

    // Check if root is a JsonObject
    std::shared_ptr<JsonObject> jsonObject = std::dynamic_pointer_cast<JsonObject>(root);
    if (!jsonObject) {
        std::cerr << "[JSON parser] Root is not a JsonObject." << std::endl;
        return 1;
    }

    std::cout << *jsonObject;

    return 0;
}


/*
Test task 1
JSON Expression Parser

Write a C++ console application that allows evaluating expressions on a JSON file.

Consider the following scenario:
###################################################
# test.json is a JSON file with the content:
# {"a": { "b": [ 1, 2, { "c": "test" }, [11, 12] ]}}
# ./json_eval is the console application
###################################################

The application should support:

    Trivial expressions with JSON paths:
    $ ./json_eval test.json "a.b[1]"
    2
    $ ./json_eval test.json "a.b[2].c"
    test
    $ ./json_eval test.json "a.b"
    [ 1, 2, { "c": "test" } ]

    Expressions in the subscript operator []:
    $ ./json_eval test.json "a.b[a.b[1]].c"
    test

    Intrinsic functions: min, max, size:
    min, max return the min, max element from the passed array or arguments.
    $ ./json_eval test.json "max(a.b[0], a.b[1])"
    2
    $ ./json_eval test.json "min(a.b[3])"
    11
    size - returns size of the passed object, array or string
    $ ./json_eval test.json "size(a)"
    1
    $ ./json_eval test.json "size(a.b)"
    4
    $ ./json_eval test.json "size(a.b[a.b[1]].c)"
    4
    Number literals:
    $ ./json_eval test.json "max(a.b[0], 10, a.b[1], 15)"
    15

Feel free to implement any suitable interface for the application.

Requirements:

    The implementation should be as fast as possible.
    Implement reasonable error reporting. For example, if the JSON doesn't have a required field, the application should report an error. If the JSON is invalid, the library should report that as well. Note that the expression can also be invalid.
    Add automated tests.
    Use any modern C++ standard: C++11, C++14, C++17, or C++20.
    You may use any functions from the C++ standard library.
    It is allowed to use a third-party library only for writing unit tests, such as GTest or Catch2, etc. It is forbidden to use third-party libraries for any other purposes, such as parsing JSON.
    Include a clear README with instructions on how to build the application.

Optional:

    You may try to speed up the implementation using multithreading.
    Support arithmetic binary operators: +, -, *, /:
    $ ./json_eval test.json "a.b[0] + a.b[1]"
    3
    Come up and implement additional intrinsic functions and binary operations.

The result of the task should be an archive with clear instructions on how to build the project, how to use it, and how to run the tests. Please use any of the following OS: Windows, Linux, or macOS, and any of the following compilers: MSVC, Clang, or GCC.
*/