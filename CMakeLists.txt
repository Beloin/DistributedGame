cmake_minimum_required(VERSION 3.24)
project(distributed_sys C)

set(CMAKE_C_STANDARD 11)

add_executable(
        distributed_sys
        src/main.c
        src/client.c
        src/server.c
        src/player/player.c src/player/player.h
        src/game/game.c src/game/game.h
)

add_library(
        utils_lib STATIC
        src/utils/linked_list.c
        src/utils/addr_utils.c
)

target_include_directories(utils_lib PUBLIC src/utils)
target_link_libraries(distributed_sys PUBLIC utils_lib)

option(TEST_LINKED_LIST "Linked List Tests" ON)

if (TEST_LINKED_LIST)
    add_subdirectory(tests)
endif ()