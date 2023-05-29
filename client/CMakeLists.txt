cmake_minimum_required(VERSION 3.9)
project(
    ${PROJECT_NAME}-client
    VERSION 0.0.1
    DESCRIPTION "Websocket client library"
    LANGUAGES C CXX
)

set(${PROJECT_NAME}_SRC_FILES
    include/websocketpp/client/ClientContextBuilder.hpp
    include/websocketpp/client/ClientFactory.hpp
    include/websocketpp/client/ClientVersions.hpp
    include/websocketpp/client/Consts.hpp
    include/websocketpp/client/IClient.hpp
    include/websocketpp/client/IEventHandler.hpp
    include/websocketpp/client/IMessageSender.hpp
    include/websocketpp/client/Types.hpp
    include/websocketpp/client/TypesFwd.hpp

    src/LwsAdapter/ILwsCallbackContext.hpp
    src/LwsAdapter/LwsCallback.cpp
    src/LwsAdapter/LwsCallback.hpp
    src/LwsAdapter/LwsCallbackContext.cpp
    src/LwsAdapter/LwsCallbackContext.hpp
    src/LwsAdapter/LwsContext.cpp
    src/LwsAdapter/LwsContext.hpp
    src/LwsAdapter/LwsContextDeleter.hpp
    src/LwsAdapter/LwsDataHolder.cpp
    src/LwsAdapter/LwsDataHolder.hpp
    src/LwsAdapter/LwsProtocolsFactory.cpp
    src/LwsAdapter/LwsProtocolsFactory.hpp
    src/LwsAdapter/LwsTypesFwd.hpp
    src/Client.cpp
    src/Client.hpp
    src/ClientContext.cpp
    src/ClientContext.hpp
    src/ClientContextBuilder.cpp
    src/ClientFactory.cpp
    src/IClientContext.hpp
    src/IContextVisitor.hpp
    src/IContextVisitorAcceptor.hpp
    src/TypesFwd.hpp
)

add_library(${PROJECT_NAME} STATIC ${${PROJECT_NAME}_SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    LINKER_LANGUAGE CXX
)

add_subdirectory(tests)
