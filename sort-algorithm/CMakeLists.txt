cmake_minimum_required(VERSION 2.6)

project(sortTest)

ADD_DEFINITIONS(-O3 -Wall)
add_executable(sortTest sortTest.cpp)
add_library(sort sort.cpp)

target_include_directories(sortTest PUBLIC ../common/include/gtest)
target_link_directories(sortTest PUBLIC ../common/lib/gtest)
target_link_libraries(sortTest PUBLIC libgtest.a pthread sort)
