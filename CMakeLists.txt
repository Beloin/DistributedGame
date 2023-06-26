cmake_minimum_required(VERSION 3.24)
project(distributed_sys C)

set(CMAKE_C_STANDARD 11)

add_executable(distributed_sys
        src/client.c)
