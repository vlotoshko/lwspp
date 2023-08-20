cmake_minimum_required(VERSION 3.9)
project(websockets_from_github)
include(ExternalProject)

set(WEBSOCKET_LIBRARY_LOCATION "${WEBSOCKETS_DIR}/lib")

find_library(WEBSOCKETS_LIBRARY websockets PATHS ${WEBSOCKET_LIBRARY_LOCATION} NO_DEFAULT_PATH)

if(${WEBSOCKETS_LIBRARY} STREQUAL "WEBSOCKETS_LIBRARY-NOTFOUND")
    ExternalProject_Add(websockets_external_${WEBSOCKETS_GITHUB_TAG}
        GIT_REPOSITORY https://github.com/warmcat/libwebsockets.git
        GIT_TAG ${WEBSOCKETS_GITHUB_TAG}
        CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DHAVE_SSL=ON -DCMAKE_INSTALL_PREFIX=${WEBSOCKETS_DIR}
    )
endif()
