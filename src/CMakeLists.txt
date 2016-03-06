set(SOURCE_FILES
    connection.cpp
    connection.hpp
    general.cpp
    general.hpp
    libircppclient.cpp
    libircppclient.hpp
    commands.cpp)

add_library(libircppclient ${SOURCE_FILES})
