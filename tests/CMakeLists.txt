add_executable(simple simple.cpp)
target_link_libraries(simple libircppclient ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
add_test(simple simple)
