cmake_minimum_required(VERSION 3.10)
project(dataStruct)

set(CMAKE_CXX_STANDARD 14)
add_definitions(-Wall)

add_subdirectory(b-tree)
add_subdirectory(binary-search-tree)
add_subdirectory(bloom-filter)
add_subdirectory(rb-tree)
