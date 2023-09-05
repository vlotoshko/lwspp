cmake_minimum_required(VERSION 3.9)
project(
    ${PROJECT_NAME}-client
    VERSION 0.0.1
    DESCRIPTION "Websocket client library"
    LANGUAGES C CXX
)

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

add_library(${PROJECT_NAME} STATIC ${${PROJECT_NAME}_SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
    ${WEBSOCKETS_HEADERS}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    LINKER_LANGUAGE CXX
)

install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/lwspp DESTINATION include)

add_subdirectory(tests)
