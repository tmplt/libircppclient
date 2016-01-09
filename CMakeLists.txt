# Enables some functionality
cmake_minimum_required(VERSION 3.0)
project(libircppclient VERSION 0.1.2.0)

set(DEVELOPMENT_BUILD TRUE)

set(VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.\
${PROJECT_VERSION_PATCH}.${PROJECT_VERSION_TWEAK}")

if(DEVELOPMENT_BUILD)
    set(VERSION_STRING "${VERSION_STRING}-dev")
endif()

message(STATUS "*** Generating recipe for version ${VERSION_STRING} ***")

#enable_testing()
add_definitions(-std=c++14 -Weverything
                           -Wno-c++98-compat
                           -Wno-c++98-compat-pedantic
                           -Wno-padded
                           -Wno-unused-parameter)

# For pthreads
set(THREADS_PREFER_PTHEAD_FLAG ON)

# For Boost
set(Boost_USE_STATIC_LIBS    OFF)
set(Boost_USE_MULTITHREADED  ON)

find_package(Boost COMPONENTS system REQUIRED)
find_package(Threads REQUIRED)

# For library headers
include_directories("${PROJECT_SOURCE_DIR}/src")

# Include CMakeLists.txt from src/ and tests/
add_subdirectory(src)
add_subdirectory(tests)

