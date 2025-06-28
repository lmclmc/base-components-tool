cmake_minimum_required(VERSION 3.2)

project(lmcbase VERSION 1.2.11)

add_compile_options(-W)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wconversion -Wsign-conversion   \
                     -Wshadow -Wnull-dereference -fsanitize=undefined")



include_directories(src)
include_directories(${CMAKE_INCLUDE_PATH})

add_subdirectory(src/)
add_subdirectory(demo/)

configure_file(version.h.in version.h @ONLY)
