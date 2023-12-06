cmake_minimum_required(VERSION 3.10)

project(chatroom)

add_definitions(-Wall -Wno-unused-result)
set(CMAKE_CXX_STANDARD 17)

include_directories("./")

add_executable(chatroom_server chatroom_server.cc)
target_link_libraries(chatroom_server PRIVATE chatlib)

add_executable(chatroom_client chatroom_client.cc)
target_link_libraries(chatroom_client PRIVATE chatlib)

add_library(chatlib chatlib.c)
