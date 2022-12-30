cmake_minimum_required(VERSION 2.6)

project(funcTest)

add_definitions(-O3 -Wall)
add_executable(funcTest funcTest.cpp)
add_library(customSysFunc custom_sys_func.cc)
target_include_directories(customSysFunc PUBLIC ../common/include)

target_include_directories(funcTest PUBLIC ../common/include ../common/include/gtest)
target_link_directories(funcTest PUBLIC ../common/lib/gtest)
target_link_libraries(funcTest PUBLIC libgtest.a pthread customSysFunc)
