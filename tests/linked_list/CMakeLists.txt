add_executable(linked_list_test tester.c)

target_link_libraries(linked_list_test ${PROJECT_SOURCE_DIR}/src/utils)

add_test(NAME "linked_list_tests" COMMAND "linked_list_test" WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
