add_executable(
        server_utils_test
        ./tester.cpp
)

target_link_libraries(
        server_utils_test server_lib gtest_main
)

add_test(NAME "server_utils_tests" COMMAND "server_utils_test" WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})