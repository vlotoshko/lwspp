cmake_minimum_required(VERSION 3.20)
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
    include/lwspp/server/contract/IServerControlAcceptor.hpp
    include/lwspp/server/contract/IServerLogic.hpp
    include/lwspp/server/CallbackVersions.hpp
    include/lwspp/server/Consts.hpp
    include/lwspp/server/IConnectionInfo.hpp
    include/lwspp/server/IServer.hpp
    include/lwspp/server/IServerControl.hpp
    include/lwspp/server/ServerBuilder.hpp
    include/lwspp/server/ServerLogicBase.hpp
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

add_library(${PROJECT_NAME} ${${PROJECT_NAME}_SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

lwspp_apply_target_defaults(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
    PRIVATE ${LWSPP_WEBSOCKETS_TARGET}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    SOVERSION ${MAJOR_VERSION}
    VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}
    OUTPUT_NAME ${PROJECT_NAME}
)

set(TARGET_LWSPP_SERVER "${PROJECT_NAME}" CACHE INTERNAL STRING)

include(cmake/install.cmake)

add_subdirectory(tests)
