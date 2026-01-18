cmake_minimum_required(VERSION 3.20)
project(
    lwspp
    VERSION 0.0.1
    DESCRIPTION "lwspp - C++ wrapper for the libwebsockets library"
    LANGUAGES C CXX
)

include(cmake/lwspp-targets.cmake)

option(LWSPP_BUILD_CLIENT "Enable or disable the build of the lwspp client library." ON)
option(LWSPP_BUILD_SERVER "Enable or disable the build of the lwspp server library." ON)
option(BUILD_SHARED_LIBS "Build lwspp libraries as shared libraries." OFF)
option(LWSPP_BUILD_EXAMPLES "Enable or disable the build of example applications." OFF)
option(LWSPP_BUILD_INTEGRATION_TESTS "Enable or disable the build of the integration tests application." OFF)

set(LWSPP_TESTS_INSTALL_DIR "" CACHE PATH "Specify the directory where the tests should be installed.
If you set this option, it will add the target install-lwspp-tests.")
set(LWSPP_EXAMPLES_INSTALL_DIR "" CACHE PATH "Specify the path where examples should be installed after the build.
Setting this option adds the corresponding install-'project name' target for each example.")
set(LWSPP_LIBWEBSOCKETS_ROOT "" CACHE PATH "Custom libwebsockets root containing include and lib directories.")
set(LWSPP_OPENSSL_ROOT "" CACHE PATH "Custom OpenSSL root containing include and lib directories.")
set(LWSPP_CATCH2_ROOT "" CACHE PATH "Custom Catch2 root for CMake package discovery.")
set(LWSPP_WEBSOCKETS_TARGET "" CACHE STRING "Override libwebsockets imported target name.")

if (NOT LWSPP_BUILD_CLIENT AND NOT LWSPP_BUILD_SERVER)
    message(FATAL_ERROR "No libraries selected for building. Please specify 'client', 'server', or both.")
endif()

if (LWSPP_BUILD_INTEGRATION_TESTS AND (NOT LWSPP_BUILD_CLIENT OR NOT LWSPP_BUILD_SERVER))
    message(FATAL_ERROR "Integration tests require both 'client' and 'server' libraries to be built.")
endif()

if (LWSPP_BUILD_EXAMPLES AND (NOT LWSPP_BUILD_CLIENT OR NOT LWSPP_BUILD_SERVER))
    message(FATAL_ERROR "Example applications require both 'client' and 'server' libraries to be built.")
endif()


if(NOT "${LWSPP_OPENSSL_ROOT}" STREQUAL "")
    list(APPEND CMAKE_PREFIX_PATH "${LWSPP_OPENSSL_ROOT}")
    set(OPENSSL_ROOT_DIR "${LWSPP_OPENSSL_ROOT}")
endif()

if(NOT "${LWSPP_CATCH2_ROOT}" STREQUAL "")
    list(APPEND CMAKE_PREFIX_PATH "${LWSPP_CATCH2_ROOT}")
endif()

if(NOT "${LWSPP_LIBWEBSOCKETS_ROOT}" STREQUAL "")
    list(APPEND CMAKE_PREFIX_PATH "${LWSPP_LIBWEBSOCKETS_ROOT}")
endif()

find_package(libwebsockets REQUIRED)

if(NOT "${LWSPP_WEBSOCKETS_TARGET}" STREQUAL "")
    if(NOT TARGET ${LWSPP_WEBSOCKETS_TARGET})
        message(FATAL_ERROR "LWSPP_WEBSOCKETS_TARGET is set but '${LWSPP_WEBSOCKETS_TARGET}' is not a known target.")
    endif()
elseif(TARGET websockets)
    set(LWSPP_WEBSOCKETS_TARGET websockets)
elseif(TARGET libwebsockets::libwebsockets)
    set(LWSPP_WEBSOCKETS_TARGET libwebsockets::libwebsockets)
elseif(TARGET libwebsockets::websockets)
    set(LWSPP_WEBSOCKETS_TARGET libwebsockets::websockets)
else()
    message(FATAL_ERROR "libwebsockets package found but no supported target was exported.")
endif()

add_library(lwspp_common_includes INTERFACE)
target_include_directories(lwspp_common_includes INTERFACE
    ${PROJECT_SOURCE_DIR}/client/include
    ${PROJECT_SOURCE_DIR}/server/include
)

if(LWSPP_BUILD_CLIENT)
    add_subdirectory(client)
endif()

if(LWSPP_BUILD_SERVER)
    add_subdirectory(server)
endif()

if(LWSPP_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(LWSPP_BUILD_INTEGRATION_TESTS)
    add_subdirectory(tests)
endif()
