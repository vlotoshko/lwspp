cmake_minimum_required(VERSION 3.9)
project(
    ${PROJECT_NAME}-server
    VERSION 0.0.1
    DESCRIPTION "Websocket server library"
    LANGUAGES C CXX
)

set(${PROJECT_NAME}_SRC_FILES
    include/websocketpp/server/IEventHandler.hpp
    include/websocketpp/server/IMessageSender.hpp
    include/websocketpp/server/IServer.hpp
    include/websocketpp/server/ServerContextBuilder.hpp
    include/websocketpp/server/ServerFactory.hpp
    include/websocketpp/server/ServerVersions.hpp
    include/websocketpp/server/Types.hpp
    include/websocketpp/server/TypesFwd.hpp

    src/LwsAdapter/ILwsCallbackContext.hpp
    src/LwsAdapter/ILwsCallbackNotifier.hpp
    src/LwsAdapter/ILwsSession.hpp
    src/LwsAdapter/ILwsSessions.hpp
    src/LwsAdapter/LwsCallback.cpp
    src/LwsAdapter/LwsCallback.hpp
    src/LwsAdapter/LwsCallbackContext.cpp
    src/LwsAdapter/LwsCallbackContext.hpp
    src/LwsAdapter/LwsContext.cpp
    src/LwsAdapter/LwsContext.hpp
    src/LwsAdapter/LwsContextDeleter.hpp
    src/LwsAdapter/LwsDataHolder.cpp
    src/LwsAdapter/LwsDataHolder.hpp
    src/LwsAdapter/LwsMessageSender.cpp
    src/LwsAdapter/LwsMessageSender.hpp
    src/LwsAdapter/LwsProtocolsFactory.cpp
    src/LwsAdapter/LwsProtocolsFactory.hpp
    src/LwsAdapter/LwsSession.cpp
    src/LwsAdapter/LwsSession.hpp
    src/LwsAdapter/LwsSessions.cpp
    src/LwsAdapter/LwsSessions.hpp
    src/LwsAdapter/LwsTypesFwd.hpp

    src/Consts.hpp
    src/IContextVisitor.hpp
    src/IContextVisitorAcceptor.hpp
    src/IServerContext.hpp
    src/Server.cpp
    src/Server.hpp
    src/ServerContext.cpp
    src/ServerContext.hpp
    src/ServerContextBuilder.cpp
    src/ServerFactory.cpp
    src/TypesFwd.hpp
)

add_library(${PROJECT_NAME} STATIC ${${PROJECT_NAME}_SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE websockets
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON1
    CXX_EXTENSIONS OFF
    LINKER_LANGUAGE CXX
)

add_subdirectory(tests)
