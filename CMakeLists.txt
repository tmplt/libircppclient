# Enables some functionality
cmake_minimum_required(VERSION 3.0)
project(libircppclient VERSION 0.1.3.0)

set(DEVELOPMENT_BUILD TRUE)

set(VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.\
${PROJECT_VERSION_PATCH}.${PROJECT_VERSION_TWEAK}")

if (DEVELOPMENT_BUILD)
    set(VERSION_STRING "${VERSION_STRING}-dev")
endif()

add_definitions(-std=c++14)

# Linux Clang + AppleClang
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")

    add_definitions(-Weverything -Wno-c++98-compat
                                 -Wno-c++98-compat-pedantic
                                 -Wno-padded
                                 -Wno-unused-parameter)

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_definitions(-Wall)
endif()

# For pthreads.
set(THREADS_PREFER_PTHEAD_FLAG ON)

# For Boost.
set(Boost_USE_STATIC_LIBS    OFF)
set(Boost_USE_MULTITHREADED  ON)

find_package(Boost COMPONENTS system REQUIRED)
find_package(Threads REQUIRED)

message("
         ---------------------------
         ${PROJECT_NAME} v${VERSION_STRING}
         ---------------------------

         Compiler  | ${CMAKE_CXX_COMPILER_ID}-${CMAKE_CXX_COMPILER_VERSION}
         Libraries | Boost-${Boost_MAJOR_VERSION}.${Boost_MINOR_VERSION}.${Boost_SUBMINOR_VERSION}
         C++ flags | ${CMAKE_CXX_FLAGS}
         Build dir | ${CMAKE_BINARY_DIR}
")

# Include CMakeLists.txt from src/ and tests/
add_subdirectory(src)
add_subdirectory(tests)

