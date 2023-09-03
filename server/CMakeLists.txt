cmake_minimum_required(VERSION 3.9)
project(
    ${PROJECT_NAME}-server
    VERSION 0.0.1
    DESCRIPTION "Websocket server library"
    LANGUAGES C CXX
)

set(${PROJECT_NAME}_SRC_FILES
    include/easywebsocket/server/CallbackVersions.hpp
    include/easywebsocket/server/Consts.hpp
    include/easywebsocket/server/EventHandlerBase.hpp
    include/easywebsocket/server/IEventHandler.hpp
    include/easywebsocket/server/IMessageSender.hpp
    include/easywebsocket/server/IMessageSenderAcceptor.hpp
    include/easywebsocket/server/IServer.hpp
    include/easywebsocket/server/ISessionInfo.hpp
    include/easywebsocket/server/ServerBuilder.hpp
    include/easywebsocket/server/SslSettingsBuilder.hpp
    include/easywebsocket/server/Types.hpp
    include/easywebsocket/server/TypesFwd.hpp

    src/LwsAdapter/ILwsCallbackContext.hpp
    src/LwsAdapter/ILwsCallbackNotifier.hpp
    src/LwsAdapter/ILwsSession.hpp
    src/LwsAdapter/ILwsSessions.hpp
    src/LwsAdapter/LwsCallback.cpp
    src/LwsAdapter/LwsCallback.hpp
    src/LwsAdapter/LwsCallbackContext.cpp
    src/LwsAdapter/LwsCallbackContext.hpp
    src/LwsAdapter/LwsContextDeleter.hpp
    src/LwsAdapter/LwsDataHolder.cpp
    src/LwsAdapter/LwsDataHolder.hpp
    src/LwsAdapter/LwsMessageSender.cpp
    src/LwsAdapter/LwsMessageSender.hpp
    src/LwsAdapter/LwsProtocolsFactory.cpp
    src/LwsAdapter/LwsProtocolsFactory.hpp
    src/LwsAdapter/LwsServer.cpp
    src/LwsAdapter/LwsServer.hpp
    src/LwsAdapter/LwsSession.cpp
    src/LwsAdapter/LwsSession.hpp
    src/LwsAdapter/LwsSessions.cpp
    src/LwsAdapter/LwsSessions.hpp
    src/LwsAdapter/LwsTypesFwd.hpp

    src/Consts.hpp
    src/EventHandlerBase.cpp
    src/Server.cpp
    src/Server.hpp
    src/ServerContext.hpp
    src/ServerBuilder.cpp
    src/SessionInfoExternal.cpp
    src/SessionInfoExternal.hpp
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
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/easywebsocket DESTINATION include)

add_subdirectory(tests)
