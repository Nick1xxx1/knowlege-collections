cmake_minimum_required(VERSION 3.10)

project(chatroom)

add_definitions(-Wall -Wno-unused-result)
set(CMAKE_CXX_STANDARD 17)

add_executable(chatroom chatroom.cc)
