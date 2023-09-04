cmake_minimum_required(VERSION 3.9)
project(
    ${PROJECT_NAME}-client
    VERSION 0.0.1
    DESCRIPTION "Websocket client library"
    LANGUAGES C CXX
)

set(${PROJECT_NAME}_SRC_FILES
    include/easywebsockets/client/CallbackVersions.hpp
    include/easywebsockets/client/ClientBuilder.hpp
    include/easywebsockets/client/IClient.hpp
    include/easywebsockets/client/IDataSender.hpp
    include/easywebsockets/client/IDataSenderAcceptor.hpp
    include/easywebsockets/client/EventHandlerBase.hpp
    include/easywebsockets/client/IEventHandler.hpp
    include/easywebsockets/client/ISessionInfo.hpp
    include/easywebsockets/client/SslSettingsBuilder.hpp
    include/easywebsockets/client/Types.hpp
    include/easywebsockets/client/TypesFwd.hpp

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
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/easywebsockets DESTINATION include)

add_subdirectory(tests)
