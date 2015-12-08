# Enables some functionality
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(libircppclient)

enable_testing()
add_definitions(-std=c++14 -Weverything)

# For pthreads
set(THREADS_PREFER_PTHEAD_FLAG ON)

# For Boost
set(Boost_USE_STATIC_LIBS    OFF)
set(Boost_USE_MULTITHREADED  ON)
#set(Boost_USE_STATIC_RUNTINE ON)

find_package(Boost COMPONENTS system REQUIRED)
find_package(Threads REQUIRED)

# For library headers
include_directories("${PROJECT_SOURCE_DIR}/src")

# Include CMakeLists.txt from src/ and tests/
add_subdirectory(src)
add_subdirectory(tests)

