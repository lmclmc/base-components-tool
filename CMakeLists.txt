cmake_minimum_required(VERSION 3.2)

project(lmcbase)

add_compile_options(-W)

include_directories(src)

add_subdirectory(src/)
add_subdirectory(test/)
