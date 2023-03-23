cmake_minimum_required(VERSION 3.9)
project(
    ${PROJECT_NAME}-chat
    VERSION 0.0.1
    DESCRIPTION "Websocket server for chat"
    LANGUAGES C CXX
)

set(${PROJECT_NAME}_SRC_FILES
    server/ChatMessageSender.cpp
    server/ChatMessageSender.hpp
    server/EventHandler.cpp
    server/EventHandler.hpp
    server/main.cpp
    server/Types.hpp
)

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SRC_FILES})

set(WebsocketServer_INCLUDE_DIR
    ${PROJECT_SOURCE_DIR}/../../server/include
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/server

    ${WebsocketServer_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE websocketpp-server
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON1
    CXX_EXTENSIONS OFF
    LINKER_LANGUAGE CXX
)
