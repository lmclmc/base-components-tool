cmake_minimum_required(VERSION 3.2)

project(Base)

set(CMAKE_CXX_FLAGS  "-std=c++11 -s")
add_compile_options(-W)

include_directories(src)

add_subdirectory(src/)
add_subdirectory(test/)
