cmake_minimum_required(VERSION 3.9)
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
    include/lwspp/client/CallbackVersions.hpp
    include/lwspp/client/ClientBuilder.hpp
    include/lwspp/client/IClient.hpp
    include/lwspp/client/IDataSender.hpp
    include/lwspp/client/IDataSenderAcceptor.hpp
    include/lwspp/client/EventHandlerBase.hpp
    include/lwspp/client/IEventHandler.hpp
    include/lwspp/client/ISessionInfo.hpp
    include/lwspp/client/SslSettingsBuilder.hpp
    include/lwspp/client/Types.hpp
    include/lwspp/client/TypesFwd.hpp

    src/LwsAdapter/ILwsCallbackContext.hpp
    src/LwsAdapter/ILwsSession.hpp
    src/LwsAdapter/LwsCallback.cpp
    src/LwsAdapter/LwsCallback.hpp
    src/LwsAdapter/LwsCallbackContext.cpp
    src/LwsAdapter/LwsCallbackContext.hpp
    src/LwsAdapter/LwsClient.cpp
    src/LwsAdapter/LwsClient.hpp
    src/LwsAdapter/LwsContextDeleter.hpp
    src/LwsAdapter/LwsDataHolder.cpp
    src/LwsAdapter/LwsDataHolder.hpp
    src/LwsAdapter/LwsDataSender.cpp
    src/LwsAdapter/LwsDataSender.hpp
    src/LwsAdapter/LwsProtocolsFactory.cpp
    src/LwsAdapter/LwsProtocolsFactory.hpp
    src/LwsAdapter/LwsSession.cpp
    src/LwsAdapter/LwsSession.hpp
    src/LwsAdapter/LwsTypes.hpp
    src/LwsAdapter/LwsTypesFwd.hpp

    src/Client.cpp
    src/Client.hpp
    src/ClientContext.hpp
    src/ClientBuilder.cpp
    src/Consts.hpp
    src/EventHandlerBase.cpp
    src/SessionInfo.cpp
    src/SessionInfo.hpp
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
    set(TARGET_LWSPP_CLIENT_STATIC "${PROJECT_NAME}-static" CACHE INTERNAL STRING)
    add_library(${TARGET_LWSPP_CLIENT_STATIC} STATIC $<TARGET_OBJECTS:${PROJECT_NAME}_object>)
else()
    unset(TARGET_LWSPP_CLIENT_STATIC CACHE)
endif()

if(OPTION_BUILD_SHARED)
    set(TARGET_LWSPP_CLIENT_SHARED "${PROJECT_NAME}-shared" CACHE INTERNAL STRING)
    add_library(${TARGET_LWSPP_CLIENT_SHARED} SHARED $<TARGET_OBJECTS:${PROJECT_NAME}_object>)
else()
    unset(TARGET_LWSPP_CLIENT_SHARED CACHE)
endif()

if(APPLE)
    if(OPTION_BUILD_STATIC)
        target_link_libraries(${TARGET_LWSPP_CLIENT_STATIC}
            PRIVATE websockets
        )
    endif()

    if(OPTION_BUILD_SHARED)
        target_link_libraries(${TARGET_LWSPP_CLIENT_SHARED}
            PRIVATE websockets
        )
    endif()
endif()

set_target_properties(${TARGET_LWSPP_CLIENT_STATIC} ${TARGET_LWSPP_CLIENT_SHARED} PROPERTIES
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
