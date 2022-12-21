cmake_minimum_required(VERSION 3.10)

project(testSort)

add_definitions(-O3 -Wall)
add_executable(testSort test_sort.cc)

target_include_directories(testSort PUBLIC ${PROJECT_SOURCE_DIR} ../common/include ../common/include/gtest)
target_link_directories(testSort PUBLIC ../common/lib/gtest)
target_link_libraries(testSort PUBLIC libgtest.a pthread)
