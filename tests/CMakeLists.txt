add_executable(simple simple.cpp)
target_link_libraries(simple ${LINK_LIBS})
add_test(simple simple)

# TODO: Fix this check
#if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/unit_testing.cpp")
    add_executable(ut unit_test.cpp)
    target_link_libraries(ut ${LINK_LIBS})
#endif()
