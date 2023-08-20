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
set(INSTALL_TESTS "" CACHE PATH "Path to install tests after build")
set(INSTALL_EXAMPLES "" CACHE PATH "Path to install examples after build")
set(WEBSOCKETS_GITHUB_TAG "" CACHE STRING
    "If set the websocket library of the correspondent version from github will be downloaded and used.
Example of version: v4.3.0")

if(NOT OPTION_BUILD_CLIENT AND NOT OPTION_BUILD_SERVER)
    message(FATAL_ERROR "Nothing to build. Please specify what to build - server, client, or both.")
endif()

if(OPTION_BUILD_INTEGRATION_TESTS AND (NOT OPTION_BUILD_CLIENT OR NOT OPTION_BUILD_SERVER))
    message(FATAL_ERROR "To build test application please enable building for both client and server libraries.")
endif()

if(OPTION_BUILD_EXAMPLES AND (NOT OPTION_BUILD_CLIENT OR NOT OPTION_BUILD_SERVER))
    message(FATAL_ERROR "To build example applications please enable building for both client and server libraries.")
endif()

unset(WEBSOCKETS_LIBRARY CACHE)
add_library(websockets INTERFACE)

if(NOT ${WEBSOCKETS_GITHUB_TAG} STREQUAL "")
    set(WEBSOCKETS_DIR ${CMAKE_BINARY_DIR}/websockets/${WEBSOCKETS_GITHUB_TAG})
    set(WEBSOCKETS_HEADERS ${WEBSOCKETS_DIR}/include)

    add_subdirectory(websockets EXCLUDE_FROM_ALL)

    # Configure websockets
    execute_process(
        COMMAND ${CMAKE_COMMAND} ${CMAKE_SOURCE_DIR}/websockets
        -DWEBSOCKETS_GITHUB_TAG=${WEBSOCKETS_GITHUB_TAG}
        -DWEBSOCKETS_DIR=${WEBSOCKETS_DIR}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/websockets)

    # Build websockets
    execute_process(COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}/websockets)

    # Install websockets
    execute_process(COMMAND ${CMAKE_COMMAND} --install ${CMAKE_BINARY_DIR}/websockets)
endif()

if(NOT ${WEBSOCKETS_GITHUB_TAG} STREQUAL "")
    find_library(WEBSOCKETS_LIBRARY websockets PATHS "${WEBSOCKETS_DIR}/lib" REQUIRED NO_DEFAULT_PATH)
else()
    find_library(WEBSOCKETS_LIBRARY websockets REQUIRED)
endif()

target_link_libraries(websockets INTERFACE ${WEBSOCKETS_LIBRARY})

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
