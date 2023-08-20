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
    ${WebsocketServer_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE easywebsocket-server
    PRIVATE websockets
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    LINKER_LANGUAGE CXX
)

if(NOT "${INSTALL_EXAMPLES}" STREQUAL "")
    if(NOT ${WEBSOCKETS_DIR} STREQUAL "")
        set_target_properties(${PROJECT_NAME} PROPERTIES
            INSTALL_RPATH "${WEBSOCKETS_DIR}/lib"
        )
    endif()

    add_custom_target(${PROJECT_NAME}_install ALL)

    add_custom_command(
        TARGET ${PROJECT_NAME}_install
        COMMAND "${CMAKE_COMMAND}"
        --install "${CMAKE_BINARY_DIR}"
        --component ${PROJECT_NAME}
        --verbose
    )

    install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${INSTALL_EXAMPLES}
        COMPONENT ${PROJECT_NAME}
    )

    install(FILES ${PROJECT_SOURCE_DIR}/client.html
        DESTINATION ${INSTALL_EXAMPLES}
        COMPONENT ${PROJECT_NAME}
    )
endif()
