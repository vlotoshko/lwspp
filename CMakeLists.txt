cmake_minimum_required(VERSION 3.9)
project(
    lwspp
    VERSION 0.0.1
    DESCRIPTION "lwspp - C++ wrapper for the libwebsockets library"
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -fPIC")
set(CMAKE_EXE_LINKER_FLAGS ${LINKER_FLAGS})

# Adding the CXX standard value into the CMAKE_CXX_STANDARD because Clang ignores the CXX_STANDARD
# value for a mscOS. Bug: https://gitlab.kitware.com/cmake/cmake/-/issues/15943
if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
endif()

option(OPTION_BUILD_CLIENT "Enable or disable the build of the lwspp client library." ON)
option(OPTION_BUILD_SERVER "Enable or disable the build of the lwspp server library." ON)
option(OPTION_BUILD_SHARED "Enable or disable the build of shared lwspp libraries." ON)
option(OPTION_BUILD_STATIC "Enable or disable the build of static lwspp libraries." OFF)
option(OPTION_BUILD_EXAMPLES "Enable or disable the build of example applications." OFF)
option(OPTION_BUILD_INTEGRATION_TESTS  "Enable or disable the build of the integration tests application." OFF)
set(INSTALL_TESTS "" CACHE PATH "Specify the directory where the tests should be installed.
If you set this option, it will add the target install-lwspp-tests.")
set(INSTALL_EXAMPLES "" CACHE PATH "Specify the path where examples should be installed after the build.
Setting this option adds the corresponding install-'project name' target for each example.")
set(WEBSOCKETS_GITHUB_TAG "" CACHE STRING
    "If set, the libwebsockets library of the corresponding version from GitHub will be downloaded, built, and used.
Example of tag: v4.3.0")

if (NOT OPTION_BUILD_CLIENT AND NOT OPTION_BUILD_SERVER)
    message(FATAL_ERROR "No libraries selected for building. Please specify 'client', 'server', or both.")
endif()

if (NOT OPTION_BUILD_STATIC AND NOT OPTION_BUILD_SHARED)
    message(FATAL_ERROR "No library type selected for building. Please specify 'static', 'shared', or both.")
endif()

if (OPTION_BUILD_INTEGRATION_TESTS AND (NOT OPTION_BUILD_CLIENT OR NOT OPTION_BUILD_SERVER))
    message(FATAL_ERROR "Integration tests require both 'client' and 'server' libraries to be built.")
endif()

if (OPTION_BUILD_EXAMPLES AND (NOT OPTION_BUILD_CLIENT OR NOT OPTION_BUILD_SERVER))
    message(FATAL_ERROR "Example applications require both 'client' and 'server' libraries to be built.")
endif()

unset(WEBSOCKETS_LIBRARY CACHE)
add_library(websockets INTERFACE)

if(APPLE)
    set(WEBSOCKETS_HEADERS /opt/local/include)
endif()

if(NOT ${WEBSOCKETS_GITHUB_TAG} STREQUAL "")
    set(WEBSOCKETS_DIR ${CMAKE_BINARY_DIR}/websockets/${WEBSOCKETS_GITHUB_TAG})
    set(WEBSOCKETS_HEADERS ${WEBSOCKETS_DIR}/include)

    # Download libwebsockets sources
    add_subdirectory(websockets EXCLUDE_FROM_ALL)

    # Configure libwebsockets
    execute_process(
        COMMAND ${CMAKE_COMMAND} ${CMAKE_SOURCE_DIR}/websockets
        -DWEBSOCKETS_GITHUB_TAG=${WEBSOCKETS_GITHUB_TAG}
        -DWEBSOCKETS_DIR=${WEBSOCKETS_DIR}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/websockets)

    # Build libwebsockets
    execute_process(COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}/websockets)

    # Install libwebsockets
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
