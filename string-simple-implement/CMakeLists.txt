cmake_minimum_required(VERSION 2.6)

project(testString)

add_definitions(-O3 -Wall)
add_executable(testString testString.cpp)
add_library(mystring MyString.cpp)

target_include_directories(testString PUBLIC ../common/include/gtest)
target_link_directories(testString PUBLIC ../common/lib/gtest)
target_link_libraries(testString PUBLIC libgtest.a pthread mystring)
