set(SOURCE_FILES
    connection.cpp
    connection.hpp)

add_library(libircppclient ${SOURCE_FILES})

#target_include_directories(libircppclient PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
