cmake_minimum_required(VERSION 3.10)

project(testKmp)

set(CMAKE_CXX_STANDARD 14)

add_definitions(-O3 -Wall)
add_executable(testKmp test_kmp.cc)
add_library(kmp kmp.cc)
target_include_directories(kmp PUBLIC ../common/include)

target_include_directories(testKmp PUBLIC ../common/include/gtest)
target_link_directories(testKmp PUBLIC ../common/lib/gtest)
target_link_libraries(testKmp PUBLIC libgtest.a pthread kmp)
