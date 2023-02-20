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
    include/websocketpp/client/types/IClient.hpp
    include/websocketpp/client/types/TypesFwd.hpp

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
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    LINKER_LANGUAGE CXX
)

add_subdirectory(tests)
