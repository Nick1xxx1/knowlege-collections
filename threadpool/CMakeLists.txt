cmake_minimum_required(VERSION 2.6)

project(threadPool)

add_definitions(-O3 -Wall -lpthread)
add_library(threadPool threadpool.c)
