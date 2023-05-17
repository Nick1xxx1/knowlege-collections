cmake_minimum_required(VERSION 3.10)
project(network)

set(CMAKE_CXX_STANDARD 17)
add_definitions(-Wall)

add_subdirectory(tcp_client)
add_subdirectory(tcp_server)
add_subdirectory(udp_client)
add_subdirectory(udp_server)
