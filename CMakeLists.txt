# Enables some functionality
cmake_minimum_required(VERSION 3.0)
project(libircppclient VERSION 0.4.1)

set(CMAKE_BUILD_TYPE Debug)

# Get the latest abbreviated commit hash of the working branch
execute_process(
    COMMAND git rev-parse --short HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_COMMIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

set(VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.\
${PROJECT_VERSION_PATCH}")

add_definitions(-std=c++14)

# Linux Clang + AppleClang
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set(CMAKE_SHARED_LINKER_FLAGS "-Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-padded -Wno-unused-parameters")

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_SHARED_LINKER_FLAGS "-Wall")

endif()

# For pthreads.
set(THREADS_PREFER_PTHEAD_FLAG ON)

# For Boost.
set(Boost_USE_STATIC_LIBS    OFF)
set(Boost_USE_MULTITHREADED  ON)

find_package(Boost COMPONENTS system REQUIRED)
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)

set(LINK_LIBS libircppclient ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${OPENSSL_LIBRARIES})

# Include CMakeLists.txt from src/ and tests/
add_subdirectory(src)
add_subdirectory(test)

string(CONCAT PROJECT_NAME_VERSION ${PROJECT_NAME}\ v${VERSION_STRING}+${GIT_COMMIT_HASH})

message("
        -----------------------------
        ${PROJECT_NAME_VERSION}
        -----------------------------

        Compiler      | ${CMAKE_CXX_COMPILER_ID}-${CMAKE_CXX_COMPILER_VERSION}
        Libraries     | Boost-${Boost_MAJOR_VERSION}.${Boost_MINOR_VERSION}.${Boost_SUBMINOR_VERSION}
        Compile flags | ${CMAKE_SHARED_LINKER_FLAGS}
        Build dir     | ${CMAKE_BINARY_DIR}
")

