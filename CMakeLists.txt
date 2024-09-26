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

# Adding the CXX standard value into the CMAKE_CXX_STANDARD because Clang ignores the CXX_STANDARD
# value for a macOS. Bug: https://gitlab.kitware.com/cmake/cmake/-/issues/15943
if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
endif()

option(OPTION_BUILD_CLIENT "Enable or disable the build of the lwspp client library." ON)
option(OPTION_BUILD_SERVER "Enable or disable the build of the lwspp server library." ON)
option(OPTION_BUILD_SHARED "Enable or disable the build of shared lwspp libraries." ON)
option(OPTION_BUILD_STATIC "Enable or disable the build of static lwspp libraries." ON)
option(OPTION_BUILD_EXAMPLES "Enable or disable the build of example applications." OFF)
option(OPTION_BUILD_INTEGRATION_TESTS  "Enable or disable the build of the integration tests application." OFF)

set(TESTS_INSTALL_DIR "" CACHE PATH "Specify the directory where the tests should be installed.
If you set this option, it will add the target install-lwspp-tests.")
set(EXAMPLES_INSTALL_DIR "" CACHE PATH "Specify the path where examples should be installed after the build.
Setting this option adds the corresponding install-'project name' target for each example.")
set(WEBSOCKETS_SEARCH_DIR "" CACHE PATH "The custom libwebsockets library location.
Expected that the directories ./include and ./lib are exsist.")

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

if(APPLE)
    set(WEBSOCKETS_HEADERS /opt/local/include)
endif()

unset(WEBSOCKETS_LIBRARY CACHE)
if(NOT "${WEBSOCKETS_SEARCH_DIR}" STREQUAL "")
    set(WEBSOCKETS_HEADERS ${WEBSOCKETS_SEARCH_DIR}/include)
    list(APPEND CMAKE_PREFIX_PATH "${WEBSOCKETS_SEARCH_DIR}")
endif()

find_library(WEBSOCKETS_LIBRARY websockets REQUIRED)

message(STATUS "Found libwebsockets library: " ${WEBSOCKETS_LIBRARY})

add_library(websockets INTERFACE)
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
