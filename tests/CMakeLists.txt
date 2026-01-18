add_executable(test_linked_list
    test_linked_list.c
)

target_include_directories(test_linked_list PRIVATE
    ${CMAKE_SOURCE_DIR}
)

target_link_libraries(test_linked_list
    generichelper
)
