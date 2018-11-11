cmake_minimum_required(VERSION 3.12)
project(assignment3_client C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(assignment3_client
        client.c)
