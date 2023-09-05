cmake_minimum_required(VERSION 3.9)
project(
    ${PROJECT_NAME}-helloworld

    VERSION 0.0.1
    LANGUAGES C CXX
)

set(${PROJECT_NAME}_SRC_FILES
    main.cpp
)

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SRC_FILES})

set(WebsocketClient_INCLUDE_DIR
    ${PROJECT_SOURCE_DIR}/../../client/include
)

set(WebsocketServer_INCLUDE_DIR
    ${PROJECT_SOURCE_DIR}/../../server/include
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${WebsocketClient_INCLUDE_DIR}
    ${WebsocketServer_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE lwspp-client
    PRIVATE lwspp-server
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

    add_custom_target(${PROJECT_NAME}_install DEPENDS ${PROJECT_NAME})

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
endif()
