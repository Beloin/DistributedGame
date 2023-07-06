add_executable(linked_list_test tester.c LinkedListTests2.hpp)

target_link_libraries(linked_list_test PUBLIC utils_lib gtest_main)

add_test(NAME "linked_list_tests" COMMAND "linked_list_test" WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
