cmake_minimum_required(VERSION 3.10)

project(threadPool)

set(CMAKE_CXX_STANDARD 17)

add_definitions(-O3 -Wall -lpthread)
add_library(threadPool threadpool.c)
