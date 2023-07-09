cmake_minimum_required(VERSION 3.10)

project(algorithm)

add_definitions(-O3 -Wall)
set(CMAKE_CXX_STANDARD 17)

include_directories(
  ../common/include
  ../common/include/gtest
)

link_directories(
  ../common/lib/gtest
)

add_subdirectory(kmp-algorithm)
add_subdirectory(sort-algorithm)
