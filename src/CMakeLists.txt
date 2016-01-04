set(SOURCE_FILES
    connection.cpp
    connection.hpp
    libircppclient.cpp
    libircppclient.hpp commands.cpp)

add_library(libircppclient ${SOURCE_FILES})
