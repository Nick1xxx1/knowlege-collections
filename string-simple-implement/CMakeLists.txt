cmake_minimum_required(VERSION 3.10)

project(testString)

set(CMAKE_CXX_STANDARD 17)

add_definitions(-O3 -Wall)
add_executable(testString test_string.cc)
add_library(mystring my_string.cc)
target_include_directories(mystring PUBLIC ../common/include)

target_include_directories(testString PUBLIC ../common/include ../common/include/gtest)
target_link_directories(testString PUBLIC ../common/lib/gtest)
target_link_libraries(testString PUBLIC libgtest.a pthread mystring)
