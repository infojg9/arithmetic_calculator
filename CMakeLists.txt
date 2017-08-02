# Copyright: jg.info9@gmail.com

cmake_minimum_required(VERSION 3.5)
project(calculator)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -ggdb3 -g3 -Wall -Wextra -O0")
set(CMAKE_VERBOSE_MAKEFILE ON)

set(SOURCE_FILES ArithmeticCalculator.cpp ArithmeticCalculator.h main.cpp)
add_executable(calculator ${SOURCE_FILES})
