cmake_minimum_required(VERSION 3.30)
project(Calc_boolean_expression)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -Wall -Werror -pedantic-errors -g -Wextra -fsanitize=address,leak,undefined -Wno-error=unused-parameter")
add_executable(Calc_boolean_expression boolcalc.cpp boolexpr.cpp boolexpr.h
        Error.h)
add_executable(test test.cpp boolexpr.cpp boolexpr.cpp boolexpr.h Error.h)
target_link_libraries(test gtest gtest_main)