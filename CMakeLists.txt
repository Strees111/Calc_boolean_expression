cmake_minimum_required(VERSION 3.27)
project(Calc_boolean_expression)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "$CMAKE_CXX_FLAGS -O3")
# Найти Python и pybind11
find_package(pybind11 CONFIG REQUIRED)

# Create an extension module
pybind11_add_module(boolcalc
        boolcalc.cpp
        boolexpr.cpp
        boolexpr.h
        Error.h)
target_link_libraries(boolcalc PUBLIC pybind11::module)