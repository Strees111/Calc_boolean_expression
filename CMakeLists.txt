cmake_minimum_required(VERSION 3.25)
project(Calc_boolean_expression)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

# Найти Python и pybind11
find_package(pybind11 CONFIG REQUIRED)
find_package(GTest REQUIRED)

add_executable(Test 
        src/boolexpr.cpp  
        src/boolexpr.h 
        src/Error.h 
        src/test.cpp)
target_link_libraries(Test gtest gtest_main pthread)


pybind11_add_module(boolcalc
        src/boolcalc.cpp
        src/boolexpr.cpp
        src/boolexpr.h
        src/Error.h)
target_link_libraries(boolcalc PUBLIC pybind11::module)


enable_testing()
add_test(NAME BoolCalcTests COMMAND Test)


option(BUILD_EXECUTABLE "Build the executable" ON)
if(NOT BUILD_EXECUTABLE)
    set_target_properties(Test PROPERTIES EXCLUDE_FROM_ALL TRUE)
endif()