cmake_minimum_required(VERSION 3.24)
project(distributed_sys C)

set(CMAKE_C_STANDARD 11)

add_executable(
        distributed_sys
        src/main.c
        src/client.c
        src/server.c
        src/utils/addr_utils.c src/utils/addr_utils.h src/player/player.c src/player/player.h src/game/game.c src/game/game.h)
