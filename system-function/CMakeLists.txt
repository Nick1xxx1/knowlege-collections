cmake_minimum_required(VERSION 3.10)

project(funcTest)

set(CMAKE_CXX_STANDARD 17)

add_definitions(-O3 -Wall)
add_executable(funcTest funcTest.cpp)
add_library(customSysFunc custom_sys_func.cc)
target_include_directories(customSysFunc PUBLIC ../common/include)

target_include_directories(funcTest PUBLIC ../common/include ../common/include/gtest)
target_link_directories(funcTest PUBLIC ../common/lib/gtest)
target_link_libraries(funcTest PUBLIC libgtest.a pthread customSysFunc)
