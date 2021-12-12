cmake_minimum_required(VERSION 3.5)

project(calc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(Catch2)
include_directories(Catch2/single_include/catch2)
add_executable(calc main.cpp Tokenizer.cpp Token.cpp TokenVisitor.cpp Stringify.cpp ParserVisitor.cpp RpnCalculator.cpp)
add_executable(test test.cpp Tokenizer.cpp Token.cpp TokenVisitor.cpp Stringify.cpp ParserVisitor.cpp RpnCalculator.cpp)
