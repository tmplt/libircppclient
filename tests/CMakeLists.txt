add_executable(simple simple.cpp)
target_link_libraries(simple libircppclient ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
add_test(simple simple)

if (EXISTS "unit_testing.cpp")
    add_executable(ut unit_testing.cpp)
endif()
