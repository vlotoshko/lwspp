cmake_minimum_required(VERSION 3.20)
project(
    ${PROJECT_NAME}-client
    VERSION 0.0.0
    DESCRIPTION "Websocket client library"
    LANGUAGES C CXX
)

set(MAJOR_VERSION 0)
set(MINOR_VERSION 0)
set(PATCH_VERSION 0)

set(${PROJECT_NAME}_SRC_FILES
    include/lwspp/client/contract/IClientControlAcceptor.hpp
    include/lwspp/client/contract/IClientLogic.hpp
    include/lwspp/client/CallbackVersions.hpp
    include/lwspp/client/ClientBuilder.hpp
    include/lwspp/client/ClientLogicBase.hpp
    include/lwspp/client/IClient.hpp
    include/lwspp/client/IClientControl.hpp
    include/lwspp/client/IConnectionInfo.hpp
    include/lwspp/client/SslSettingsBuilder.hpp
    include/lwspp/client/Types.hpp
    include/lwspp/client/TypesFwd.hpp

    src/LwsAdapter/ILwsCallbackContext.hpp
    src/LwsAdapter/ILwsConnection.hpp
    src/LwsAdapter/LwsCallback.cpp
    src/LwsAdapter/LwsCallback.hpp
    src/LwsAdapter/LwsCallbackContext.cpp
    src/LwsAdapter/LwsCallbackContext.hpp
    src/LwsAdapter/LwsClient.cpp
    src/LwsAdapter/LwsClient.hpp
    src/LwsAdapter/LwsClientControl.cpp
    src/LwsAdapter/LwsClientControl.hpp
    src/LwsAdapter/LwsConnection.cpp
    src/LwsAdapter/LwsConnection.hpp
    src/LwsAdapter/LwsContextDeleter.hpp
    src/LwsAdapter/LwsDataHolder.cpp
    src/LwsAdapter/LwsDataHolder.hpp
    src/LwsAdapter/LwsProtocolsFactory.cpp
    src/LwsAdapter/LwsProtocolsFactory.hpp
    src/LwsAdapter/LwsTypes.hpp
    src/LwsAdapter/LwsTypesFwd.hpp

    src/Client.cpp
    src/Client.hpp
    src/ClientContext.hpp
    src/ClientBuilder.cpp
    src/ConnectionInfo.hpp
    src/Consts.hpp
    src/ClientLogicBase.cpp
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

set(TARGET_LWSPP_CLIENT "${PROJECT_NAME}" CACHE INTERNAL STRING)

include(cmake/install.cmake)

add_subdirectory(tests)
