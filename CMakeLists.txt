cmake_minimum_required(VERSION 3.9)
project(
    easywebsocket
    VERSION 0.0.1
    DESCRIPTION "Easywebsocket - C++ wrapper for the libwebsocket library"
    LANGUAGES C CXX
)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic")
set(CMAKE_EXE_LINKER_FLAGS ${LINKER_FLAGS})

option(OPTION_BUILD_CLIENT "Build easywebsocket client library" ON)
option(OPTION_BUILD_SERVER "Build easywebsocket server library" ON)
option(OPTION_BUILD_EXAMPLES "Build examples" ON)
option(OPTION_BUILD_INTEGRATION_TESTS  "Build integration tests application" ON)
set(INSTALL_PATH_TESTS "" CACHE PATH "Path to install tests after build")
set(INSTALL_PATH_EXAMPLES "" CACHE PATH "Path to install examples after build")

if(NOT OPTION_BUILD_CLIENT AND NOT OPTION_BUILD_SERVER)
    message(FATAL_ERROR "Nothing to build. Please specify what to build - server, client, or both.")
endif()

if(OPTION_BUILD_INTEGRATION_TESTS AND (NOT OPTION_BUILD_CLIENT OR NOT OPTION_BUILD_SERVER))
    message(FATAL_ERROR "To build test application please enable building for both client and server libraries.")
endif()

if(OPTION_BUILD_EXAMPLES AND (NOT OPTION_BUILD_CLIENT OR NOT OPTION_BUILD_SERVER))
    message(FATAL_ERROR "To build example applications please enable building for both client and server libraries.")
endif()

if(OPTION_BUILD_CLIENT)
    add_subdirectory(client)
endif()

if(OPTION_BUILD_SERVER)
    add_subdirectory(server)
endif()

if(OPTION_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(OPTION_BUILD_INTEGRATION_TESTS)
    add_subdirectory(tests)
endif()
