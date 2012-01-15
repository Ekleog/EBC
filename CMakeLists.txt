cmake_minimum_required (VERSION 2.8.7)
project ("bbc")

set (CMAKE_CXX_FLAGS "-std=c++0x -ggdb3 -Wall -Wextra -pedantic")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")

add_executable (
    test
    "src/AST.cpp"
    "src/helpers.cpp"
    "src/parse.cpp"
    "src/test.cpp"
    "src/Tokenizer.cpp"
    "src/Visitor.cpp"
)
