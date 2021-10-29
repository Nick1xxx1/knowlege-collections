cmake_minimum_required(VERSION 2.6)

project(testKmp)

add_definitions(-O3 -Wall)
add_executable(testKmp testKmp.cpp)
add_library(kmp kmp.c)

target_include_directories(testKmp PUBLIC ../common/include/gtest)
target_link_directories(testKmp PUBLIC ../common/lib/gtest)
target_link_libraries(testKmp PUBLIC libgtest.a pthread kmp)
