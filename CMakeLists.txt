cmake_minimum_required(VERSION 3.2)

project(lmcbase VERSION 3.2.11)

add_compile_options(-W)

set(CMAKE_CXX_FLAGS "-std=c++14 -Wall -Wextra -Wconversion -Wsign-conversion\
                     -Wshadow -Wnull-dereference")



include_directories(src)
include_directories(${CMAKE_INCLUDE_PATH})

add_subdirectory(src/)

if(IS_BUILD_DEMO)
  add_subdirectory(demo/)
endif()

configure_file(version.h.in version.h @ONLY)
