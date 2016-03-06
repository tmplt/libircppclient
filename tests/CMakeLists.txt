#add_executable(con_only connection_only.cpp)
add_executable(simple simple.cpp)

#target_link_libraries(con_only libircppclient ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(simple libircppclient ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

#add_test(exec_path con_only)
add_test(simple simple)
