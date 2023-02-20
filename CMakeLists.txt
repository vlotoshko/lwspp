cmake_minimum_required(VERSION 3.9)
project(
    websocketpp
    VERSION 0.0.1
    DESCRIPTION "C++ wrapper for the libwebsocket library"
    LANGUAGES C CXX
)

if(NOT WIN32)
    set(LINKER_FLAGS "-pthread")
endif (NOT WIN32)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic")
set(CMAKE_EXE_LINKER_FLAGS ${LINKER_FLAGS})

option(OPTION_BUILD_CLIENT "Build websocketpp client library" ON)
option(OPTION_BUILD_SERVER "Build websocketpp server library" ON)
option(OPTION_BUILD_EXAMPLES "Build websocketpp server library" ON)
option(OPTION_BUILD_INTEGRATION_TESTS  "Build integration tests application" ON)

if(NOT OPTION_BUILD_CLIENT AND NOT OPTION_BUILD_SERVER)
    message(FATAL_ERROR "Nothing to build. Please specify what to build - server, client, or both.")
endif()

if(OPTION_BUILD_INTEGRATION_TESTS AND (NOT OPTION_BUILD_CLIENT OR NOT OPTION_BUILD_SERVER))
    message(FATAL_ERROR "To build test application please enable building for both client and server libraries.")
endif()

if(OPTION_BUILD_EXAMPLES AND NOT OPTION_BUILD_SERVER)
    message(FATAL_ERROR "To build example applications please enable building for the server library.")
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
