cmake_minimum_required(VERSION 3.9)
project(
    ${PROJECT_NAME}-server
    VERSION 0.0.0
    DESCRIPTION "Websocket server library"
    LANGUAGES C CXX
)

set(MAJOR_VERSION 0)
set(MINOR_VERSION 0)
set(PATCH_VERSION 0)

set(${PROJECT_NAME}_SRC_FILES
    include/lwspp/server/CallbackVersions.hpp
    include/lwspp/server/Consts.hpp
    include/lwspp/server/ServerLogicBase.hpp
    include/lwspp/server/IServerControl.hpp
    include/lwspp/server/IServerControlAcceptor.hpp
    include/lwspp/server/IConnectionInfo.hpp
    include/lwspp/server/IServerLogic.hpp
    include/lwspp/server/IServer.hpp
    include/lwspp/server/ServerBuilder.hpp
    include/lwspp/server/SslSettingsBuilder.hpp
    include/lwspp/server/Types.hpp
    include/lwspp/server/TypesFwd.hpp

    src/LwsAdapter/ILwsCallbackContext.hpp
    src/LwsAdapter/ILwsCallbackNotifier.hpp
    src/LwsAdapter/ILwsConnection.hpp
    src/LwsAdapter/ILwsConnections.hpp
    src/LwsAdapter/LwsCallback.cpp
    src/LwsAdapter/LwsCallback.hpp
    src/LwsAdapter/LwsCallbackContext.cpp
    src/LwsAdapter/LwsCallbackContext.hpp
    src/LwsAdapter/LwsConnection.cpp
    src/LwsAdapter/LwsConnection.hpp
    src/LwsAdapter/LwsConnections.cpp
    src/LwsAdapter/LwsConnections.hpp
    src/LwsAdapter/LwsContextDeleter.hpp
    src/LwsAdapter/LwsDataHolder.cpp
    src/LwsAdapter/LwsDataHolder.hpp
    src/LwsAdapter/LwsProtocolsFactory.cpp
    src/LwsAdapter/LwsProtocolsFactory.hpp
    src/LwsAdapter/LwsServer.cpp
    src/LwsAdapter/LwsServer.hpp
    src/LwsAdapter/LwsServerControl.cpp
    src/LwsAdapter/LwsServerControl.hpp
    src/LwsAdapter/LwsTypes.hpp
    src/LwsAdapter/LwsTypesFwd.hpp

    src/ConnectionInfo.cpp
    src/ConnectionInfo.hpp
    src/Consts.hpp
    src/ServerLogicBase.cpp
    src/Server.cpp
    src/Server.hpp
    src/ServerContext.hpp
    src/ServerBuilder.cpp
    src/SslSettings.hpp
    src/SslSettingsBuilder.cpp
    src/TypesFwd.hpp
)

add_library(${PROJECT_NAME}_object OBJECT ${${PROJECT_NAME}_SRC_FILES})

target_include_directories(${PROJECT_NAME}_object PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
    ${WEBSOCKETS_HEADERS}
)

if(OPTION_BUILD_STATIC)
    set(TARGET_LWSPP_SERVER_STATIC "${PROJECT_NAME}-static" CACHE INTERNAL STRING)
    add_library(${TARGET_LWSPP_SERVER_STATIC} STATIC $<TARGET_OBJECTS:${PROJECT_NAME}_object>)
else()
    unset(TARGET_LWSPP_SERVER_STATIC CACHE)
endif()

if(OPTION_BUILD_SHARED)
    set(TARGET_LWSPP_SERVER_SHARED "${PROJECT_NAME}-shared" CACHE INTERNAL STRING)
    add_library(${TARGET_LWSPP_SERVER_SHARED} SHARED $<TARGET_OBJECTS:${PROJECT_NAME}_object>)
else()
    unset(TARGET_LWSPP_SERVER_SHARED CACHE)
endif()

if(APPLE)
    if(OPTION_BUILD_STATIC)
        target_link_libraries(${TARGET_LWSPP_SERVER_STATIC}
            PRIVATE websockets
        )
    endif()

    if(OPTION_BUILD_SHARED)
        target_link_libraries(${TARGET_LWSPP_SERVER_SHARED}
            PRIVATE websockets
        )
    endif()
endif()

set_target_properties(${TARGET_LWSPP_SERVER_STATIC} ${TARGET_LWSPP_SERVER_SHARED} PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    LINKER_LANGUAGE CXX
    SOVERSION ${MAJOR_VERSION}
    VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}
    OUTPUT_NAME ${PROJECT_NAME}
)

include(cmake/install.cmake)

add_subdirectory(tests)
