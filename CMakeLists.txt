cmake_minimum_required(VERSION 3.2)

project(Base)

set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_FLAGS  "-std=c++11 -s")

include_directories(src)

add_subdirectory(src/)
add_subdirectory(test/)
