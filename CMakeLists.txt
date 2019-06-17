cmake_minimum_required(VERSION 3.14)
project(hw3)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c++11 -Wall -pedantic-errors -g -Werror -DNDEBUG")
add_executable(hw3 eurovision.cpp test2.cpp)
