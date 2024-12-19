cmake_minimum_required(VERSION 3.22.1)

project(script)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(parser_generator grammar_parser.h parser_generator.cpp)
add_executable(parser parser.cpp)