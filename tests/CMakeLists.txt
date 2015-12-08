add_executable(con_only connection_only.cpp)
target_link_libraries(con_only libircppclient ${Boost_LIBRARIES})

add_test(exec_path con_only)