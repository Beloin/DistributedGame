cmake_minimum_required(VERSION 3.24)
project(distributed_sys C CXX)

set(CMAKE_C_STANDARD 11)

add_executable(
        distributed_sys
        src/main.c
        src/player/player.c src/player/player.h
        src/game/game.c src/game/game.h
)

add_library(
        utils_lib STATIC
        src/utils/linked_list.c
        src/utils/addr_utils.c
)
target_include_directories(utils_lib PUBLIC src/utils)

add_library(
        server_lib STATIC
        src/server/client.c
        src/server/server.c
        src/server/client_handler.c)
target_include_directories(
        server_lib PUBLIC
        src/server
)
target_link_libraries(server_lib PUBLIC utils_lib)

include(FetchContent)
FetchContent_Declare(
        googletest
        # Specify the commit you depend on and update it regularly.
        URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest) # This makes available the lib `gtest_main`

option(TEST_LINKED_LIST "Linked List Tests" ON)
if (TEST_LINKED_LIST)
    add_subdirectory(tests)
endif ()

target_link_libraries(distributed_sys PUBLIC utils_lib server_lib)
